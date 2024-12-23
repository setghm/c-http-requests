#include "client_socket.h"

#include <string.h>
#include <stdlib.h>

#define SOCK_FD(scs) ((int)(BaseClientSocket(scs)->_source))

ClientSocket* ClientSocket_New(void) {
    ClientSocket* cs = (ClientSocket*)malloc(sizeof(ClientSocket));

    if (cs != NULL) {
        memset(cs, 0, sizeof(ClientSocket));
    }

    return cs;
}

boolean ClientSocket_MakeSecure(ClientSocket* cs) {
    RETURN_ZERO_IF_NULL(cs);

    /*
        Create a secure layer.
    */
    cs->_secure_layer = SecureClientLayer_New();

    if (cs->_secure_layer == NULL) {
        print_log(LOG_ERROR, "ClientSocket", "Cannot create a secure layer");
        return false;
    }

    /*
        Start SSL connection.
    */
    const boolean secure_connected = SecureClientLayer_Connect(cs->_secure_layer, SOCK_FD(cs), cs->host, cs->port);

    if (secure_connected == false) {
        print_log(LOG_ERROR, "ClientSocket", "Cannot start a secure connection");
        return false;
    }

    return true;
}