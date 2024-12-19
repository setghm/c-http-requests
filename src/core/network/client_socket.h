#ifndef _REQUESTER_NETWORK_CLIENT_SOCKET_H
#define _REQUESTER_NETWORK_CLIENT_SOCKET_H

#include <stddef.h>
#include <utils/utils.h>

typedef struct _ClientSocket ClientSocket;

struct _ClientSocket {
    void* _source;

    boolean is_open;

    boolean is_read_only;
};

ClientSocket* ClientSocket_Open(const char* host, u16 port);

void ClientSocket_Close(ClientSocket* cs);

size_t ClientSocket_Read(ClientSocket* cs, const byte* buffer, size_t buffer_size);

size_t ClientSocket_Write(ClientSocket* cs, const byte* buffer, size_t buffer_size);

boolean ClientSocket_FinishWritting(ClientSocket* cs);

#endif
