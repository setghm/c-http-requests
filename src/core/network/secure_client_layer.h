#ifndef _REQUESTER_NETWORK_SECURE_CLIENT_LAYER_H
#define _REQUESTER_NETWORK_SECURE_CLIENT_LAYER_H

#include <openssl/ssl.h>
#include <openssl/err.h>

// requester
#include <utils/utils.h>

#define CLIENT_SOCKET_SAFE_READ_IF_POSSIBLE(cs, buffer, buffer_size)\
    if (cs->_secure_layer) return SecureClientLayer_Read(cs->_secure_layer, buffer, buffer_size)

#define CLIENT_SOCKET_SAFE_WRITE_IF_POSSIBLE(cs, buffer, buffer_size)\
    if (cs->_secure_layer) return SecureClientLayer_Write(cs->_secure_layer, buffer, buffer_size)

typedef struct _SecureClientLayer SecureClientLayer;

struct _SecureClientLayer {
    SSL_CTX* _context;

    SSL* _ssl;
};

void SecureClientLayers_Init();

void SecureClientLayers_Cleanup();

SecureClientLayer* SecureClientLayer_New(void);

void SecureClientLayer_Delete(SecureClientLayer* scl);

boolean SecureClientLayer_Connect(SecureClientLayer* scl, int fd, const char* hostname, u16 port);

size_t SecureClientLayer_Read(const SecureClientLayer* scl, byte* buffer, size_t buffer_size);

size_t SecureClientLayer_Write(const SecureClientLayer* scl, const byte* buffer, size_t buffer_size);

#endif
