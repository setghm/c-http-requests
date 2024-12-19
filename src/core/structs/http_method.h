#ifndef _REQUESTER_STRUCTS_HTTP_METHOD_H
#define _REQUESTER_STRUCTS_HTTP_METHOD_H

#include <stddef.h>

typedef enum _HttpMethod HttpMethod;

enum _HttpMethod {
    HTTP_METHOD_UNKNOWN,
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_PATCH,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_CONNECT,
    HTTP_METHOD_TRACE
};

HttpMethod HttpMethod_FromString(const char* src, size_t src_size);

const char* HttpMethod_ToString(HttpMethod method);

#endif