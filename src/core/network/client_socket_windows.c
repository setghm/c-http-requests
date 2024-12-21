#include "client_socket.h"

// c standard library
#include <stdio.h>

// platform-specific
#include <WS2tcpip.h>
#include <WinSock2.h>

// requester
#include <utils/utils.h>

#define WINSOCK(cs) ((SOCKET)cs->_source)

boolean ClientSocket_Connect(ClientSocket* cs, const char* host, const u16 port) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(host);
    RETURN_ZERO_IF_ZERO(port);

    /*
        Initialize Windows Socket version 2.2.
    */
    WSADATA wsa_data;
    int result;
    char port_str[6] = {0};
    struct addrinfo *addr_result = NULL, hints;

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        print_log(LOG_ERROR, "ClientSocket", "WSA initialization error");
        return false;
    }

    /*
        Try to resolve the host name into an IP address.
    */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    sprintf(port_str, "%u", port);

    result = getaddrinfo(host, port_str, &hints, &addr_result);
    if (result != 0) {
        print_log(LOG_ERROR, "ClientSocket", "getaddrinfo failed");
        WSACleanup();
        return false;
    }

    /*
        Create a new socket.
    */
    SOCKET s = INVALID_SOCKET;

    s = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);

    if (s == INVALID_SOCKET) {
        print_log(LOG_ERROR, "ClientSocket", "Cannot create the client socket");
        freeaddrinfo(addr_result);
        WSACleanup();
        return false;
    }

    /*
        Attempt connection to host.
    */
    result = connect(s, addr_result->ai_addr, (int)addr_result->ai_addrlen);

    // TODO: Check other addresses.

    if (result == SOCKET_ERROR) {
        print_log(LOG_ERROR, "ClientSocket", "Cannot connect to server");
        closesocket(s);
        freeaddrinfo(addr_result);
        WSACleanup();
        s = INVALID_SOCKET;
        return false;
    }

    freeaddrinfo(addr_result);

    /*
        Save the socket into the ClientSocket struct.
    */
    cs->_source = (void*)s;
    cs->is_open = true;
}

ClientSocket* ClientSocket_Open(const char* host, u16 port) {
    ClientSocket* cs = ClientSocket_New();

    ClientSocket_Connect(cs, host, port);

    return cs;
}

void ClientSocket_Close(ClientSocket* cs) {
    RETURN_IF_NULL(cs);

    SecureClientLayer_Delete(cs->_secure_layer);

    closesocket(WINSOCK(cs));

    WSACleanup();

    free(cs);
}

size_t ClientSocket_Read(ClientSocket* cs, byte* buffer, size_t buffer_size) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);
    RETURN_ZERO_IF_ZERO(cs->is_open);

    CLIENT_SOCKET_SAFE_READ_IF_POSSIBLE(cs, buffer, buffer_size);

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
    RETURN_ZERO_IF_ZERO(cs->is_open);
    RETURN_ZERO_IF_ZERO(!cs->is_read_only);

    CLIENT_SOCKET_SAFE_WRITE_IF_POSSIBLE(cs, buffer, buffer_size);

    i32 result;
    size_t bytes_sent;

    result = send(WINSOCK(cs), buffer, buffer_size, 0);

    if (result == SOCKET_ERROR) {
        print_log(LOG_ERROR, "ClientSocket", "Cannot send the request");

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
        print_log(LOG_ERROR, "Request", "Error closing this peer");

        fprintf(stderr, "[ERROR][WindowsSocket] Last error: %d\n", GetLastError());

        return false;
    }

    cs->is_read_only = true;

    return true;
}
