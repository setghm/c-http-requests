#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#include "client_socket.h"
#include "utils/utils.h"

#define WINSOCK(cs) ((SOCKET)cs->_source)

ClientSocket* ClientSocket_Open(const char* host, u16 port) {
    ClientSocket* cs = (ClientSocket*)malloc(sizeof(ClientSocket));

    if (cs) {
        /*
            Initialize Windows Socket version 2.2.
        */
        WSADATA wsa_data;
        int result;
        char port_str[6] = {0};
        struct addrinfo *addr_result = NULL, hints;

        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            log(LOG_ERROR, "ClientSocket", "WSA initialization error");
            return NULL;
        }

        /*
            Try to resolve the host name into an IP address.
        */
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        sprintf(port_str, "%u\0", port);

        result = getaddrinfo(host, port_str, &hints, &addr_result);
        if (result != 0) {
            log(LOG_ERROR, "ClientSocket", "getaddrinfo failed");
            WSACleanup();
            return NULL;
        }

        /*
            Create a new socket.
        */
        SOCKET s = INVALID_SOCKET;

        s = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);

        if (s == INVALID_SOCKET) {
            log(LOG_ERROR, "ClientSocket", "Cannot create the client socket");
            freeaddrinfo(addr_result);
            WSACleanup();
            return NULL;
        }

        /*
            Attempt connection to host.
        */
        result = connect(s, addr_result->ai_addr, (int)addr_result->ai_addrlen);

        // TODO: Check other addresses.

        if (result == SOCKET_ERROR) {
            log(LOG_ERROR, "ClientSocket", "Cannot connect to server");
            closesocket(s);
            freeaddrinfo(addr_result);
            WSACleanup();
            s = INVALID_SOCKET;
            return NULL;
        }

        freeaddrinfo(addr_result);

        /*
            Save the socket into the ClientSocket struct.
        */
        memset(cs, 0, sizeof(ClientSocket));

        cs->_source = s;
        cs->is_open = true;
    }

    return cs;
}

void ClientSocket_Close(ClientSocket* cs) {
    RETURN_IF_NULL(cs);

    closesocket(WINSOCK(cs));

    WSACleanup();

    free(cs);
}

size_t ClientSocket_Read(ClientSocket* cs, const byte* buffer, size_t buffer_size) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);
    RETURN_ZERO_IF_ZERO(cs->is_open);

    i32 result;
    size_t bytes_received = 0;

    result = recv(WINSOCK(cs), buffer, buffer_size, 0);

    bytes_received = (size_t)result;

    if (result == 0) {
#ifdef DEBUG
        log(LOG_DEBUG, "ClientSocket", "Connection closed");

        fprintf(stderr, "[ERROR][WindowsSocket] Last error: %d\n", GetLastError());
#endif 
    } else if (result < 0) {
#ifdef DEBUG
        log(LOG_ERROR, "ClientSocket", "Error receiving data");

        fprintf(stderr, "[ERROR][WindowsSocket] Cannot get the content: %d\n", GetLastError());
#endif
        bytes_received = 0;
    }

    return bytes_received;
}

size_t ClientSocket_Write(ClientSocket* cs, const byte* buffer, size_t buffer_size) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);

    if (cs->is_read_only || !cs->is_open) {
        return 0;
    }

    i32 result;
    size_t bytes_sent;

    result = send(WINSOCK(cs), buffer, buffer_size, 0);

    if (result == SOCKET_ERROR) {
        log(LOG_ERROR, "ClientSocket", "Cannot send the request");

        fprintf(stderr, "[ERROR][WindowsSocket] Last error: %d\n", GetLastError());

        return 0;
    }

    bytes_sent = (size_t)result;

    return bytes_sent;
}

boolean ClientSocket_FinishWritting(ClientSocket* cs) {
    RETURN_ZERO_IF_NULL(cs);

    /*
        Stop sending content.
    */
    i32 result = shutdown(WINSOCK(cs), SD_SEND);

    if (result == SOCKET_ERROR) {
        log(LOG_ERROR, "Request", "Error closing this peer");

        fprintf(stderr, "[ERROR][WindowsSocket] Last error: %d\n", GetLastError());

        return false;
    }

    cs->is_read_only = true;

    return true;
}
