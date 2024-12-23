#ifndef _REQUESTER_NETWORK_CLIENT_SOCKET_H
#define _REQUESTER_NETWORK_CLIENT_SOCKET_H

#include "secure_client_layer.h"

#include <stddef.h>
#include <utils/utils.h>

#define BaseClientSocket(derived) ((ClientSocket*)derived)

typedef struct _ClientSocket ClientSocket;

struct _ClientSocket {
    void* _source;

    boolean is_open;

    boolean is_read_only;

    SecureClientLayer* _secure_layer;

    u16 port;

    char* host;
};

ClientSocket* ClientSocket_New(void);

boolean ClientSocket_Connect(ClientSocket* cs, const char* host, u16 port);

ClientSocket* ClientSocket_Open(const char* host, u16 port);

boolean ClientSocket_MakeSecure(ClientSocket* cs);

void ClientSocket_Close(ClientSocket* cs);

size_t ClientSocket_Read(const ClientSocket* cs, byte* buffer, size_t buffer_size);

size_t ClientSocket_Write(const ClientSocket* cs, const byte* buffer, size_t buffer_size);

boolean ClientSocket_FinishWriting(const ClientSocket* cs);

#endif
