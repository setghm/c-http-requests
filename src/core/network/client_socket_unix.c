#include "client_socket.h"

// c standard library
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// platform-specific
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// requester
#include <utils/utils.h>

#define UNIXSOCK(cs) ((int)cs->_source)

boolean ClientSocket_Connect(ClientSocket* cs, const char* host, const u16 port) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(host);
    RETURN_ZERO_IF_ZERO(port);

    /*
        Create unix socket.
    */
    int fd;  // Socket file descriptor
    int result = 0;
    struct sockaddr_in server_addr;
    struct hostent* host_info;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        print_log(LOG_ERROR, "ClientSocket", "UnixSocket: Cannot create the socket");
        return false;
    }

    /*
        Attempt to resolve the host name into an IP address.
    */
    host_info = gethostbyname(host);

    if (host_info == NULL) {
        print_log(LOG_ERROR, "ClientSocket", "UnixSocket: No such host");
        return false;
    }

    /*
        Setup server structure and attempt to connect.
    */
    size_t i = 0;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    while (host_info->h_addr_list[i] != NULL) {
        const struct in_addr* address = (struct in_addr*) host_info->h_addr_list[i];

        fprintf(stderr, "[DEBUG][ClientSocket] Connecting to: %s...\n", inet_ntoa(*address));

        /*
            Copy the next address.
        */
        memcpy(&server_addr.sin_addr.s_addr, address, sizeof(server_addr.sin_addr));

        /*
            Attempt to connect to server.
        */
        result = connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

        if (result == 0) break;
        i++;
    }

    if (result != 0) {
        print_log(LOG_ERROR, "ClientSocket", "Unix Sockets: Error cannot connect to server.");
        return false;
    }

    /*
        Configure struct.
    */
    cs->_source = (void*)fd;
    cs->is_open = true;

    cs->host = strclone(host);
    cs->port = port;

    return true;
}

ClientSocket* ClientSocket_Open(const char* host, u16 port) {
    ClientSocket* cs = ClientSocket_New();

    ClientSocket_Connect(cs, host, port);

    return cs;
}

void ClientSocket_Close(ClientSocket* cs) {
    RETURN_IF_NULL(cs);

    SecureClientLayer_Delete(cs->_secure_layer);

    close(UNIXSOCK(cs));

    free(cs);
}

size_t ClientSocket_Read(const ClientSocket* cs, byte* buffer, const size_t buffer_size) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);
    RETURN_ZERO_IF_ZERO(cs->is_open);

    CLIENT_SOCKET_SAFE_READ_IF_POSSIBLE(cs, buffer, buffer_size);

    const i64 result = read(UNIXSOCK(cs), (void*)buffer, buffer_size);

    if (result < 0) {
        print_log(LOG_ERROR, "ClientSocket", "Error while receiving data");
        return 0;
    }

    return (size_t)result;
}

size_t ClientSocket_Write(const ClientSocket* cs, const byte* buffer, const size_t buffer_size) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);
    RETURN_ZERO_IF_ZERO(cs->is_open);
    RETURN_ZERO_IF_ZERO(!cs->is_read_only);

    CLIENT_SOCKET_SAFE_WRITE_IF_POSSIBLE(cs, buffer, buffer_size);

    const i64 result = write(UNIXSOCK(cs), buffer, buffer_size);

    if (result < 0) {
        print_log(LOG_ERROR, "ClientSocket", "Error while sending data");
        return 0;
    }

    return (size_t)result;
}

boolean ClientSocket_FinishWriting(const ClientSocket* cs) {
    RETURN_ZERO_IF_NULL(cs);

    const i64 result = shutdown(UNIXSOCK(cs), SHUT_WR);

    if (result < 0) {
        print_log(LOG_ERROR, "ClientSocket", "Cannot close this peer");
        return false;
    }

    return true;
}
