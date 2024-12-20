#include "client_socket.h"

// c standard library
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// platform-specific
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

// requester
#include <utils/utils.h>

#define UNIXSOCK(cs) ((int)cs->_source)

ClientSocket* ClientSocket_Open(const char* host, u16 port) {
    ClientSocket* cs = (ClientSocket*)malloc(sizeof(ClientSocket));

    if (cs) {
        /*
            Create unix socket.
        */
        int sockfd;  // Socket file descriptor
        int port;
        int n;
        int result;
        struct sockaddr_in server_addr;
        struct hostent* server;
        char buffer[256];

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) {
            print_log(LOG_ERROR, "ClientSocket", "UnixSocket: Cannot create the socket");
            return NULL;
        }

        /*
            Attempt to resolve the host name into an IP address.
        */
        server = gethostbyname(host);

        if (server == NULL) {
            print_log(LOG_ERROR, "ClientSocket", "UnixSocket: No such host");
            return NULL;
        }

        /*
            Setup server structure and attempt to connect.
        */
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        memcpy(server->h_addr_list[0], &server_addr.sin_addr.s_addr, server->h_length);

        server_addr.sin_port = htons(port);

        result = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

        if (result < 0) {
            print_log(LOG_ERROR, "ClientSocket", "Unix Sockets: Error cannot connect to server.");
            return NULL;
        }

        /*
            Configure struct.
        */
        memset(cs, 0, sizeof(ClientSocket));

        cs->_source = sockfd;
        cs->is_open = true;
    }

    return cs;
}

void ClientSocket_Close(ClientSocket* cs) {
    RETURN_IF_NULL(cs);

    close(UNIXSOCK(cs));

    free(cs);
}

size_t ClientSocket_Read(ClientSocket* cs, const byte* buffer, size_t buffer_size) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);

    i64 result = read(UNIXSOCK(cs), buffer, buffer_size);

    if (result < 0) {
        print_log(LOG_ERROR, "ClientSocket", "Error while receiving data");
        return 0;
    }

    return (size_t)result;
}

size_t ClientSocket_Write(ClientSocket* cs, const byte* buffer, size_t buffer_size) {
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);

    i64 result = write(UNIXSOCK(cs), buffer, buffer_size);

    if (result < 0) {
        print_log(LOG_ERROR, "ClientSocket", "Error while sending data");
        return 0;
    }

    return (size_t)result;
}

boolean ClientSocket_FinishWritting(ClientSocket* cs) {
    RETURN_ZERO_IF_NULL(cs);

    int result = shutdown(UNIXSOCK(cs), SHUT_WR);

    if (result < 0) {
        print_log(LOG_ERROR, "ClientSocket", "Cannot close this peer");
        return false;
    }

    return true;
}
