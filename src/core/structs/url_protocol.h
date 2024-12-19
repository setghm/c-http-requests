#ifndef _REQUESTER_STRUCTS_URL_PROTOCOL_H
#define _REQUESTER_STRUCTS_URL_PROTOCOL_H

#include <stddef.h>

typedef enum _URLProtocol URLProtocol;

enum _URLProtocol {
    URL_PROTOCOL_UNKNOWN,
    URL_PROTOCOL_HTTP,
    URL_PROTOCOL_HTTPS,
    URL_PROTOCOL_WS
};

URLProtocol URLProtocol_FromString(const char* src, size_t src_size);

const char* URLProtocol_ToString(URLProtocol protocol);

#endif