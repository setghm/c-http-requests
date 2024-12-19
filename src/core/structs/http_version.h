#ifndef _REQUESTER_STRUCTS_HTTP_VERSION_H
#define _REQUESTER_STRUCTS_HTTP_VERSION_H

#include <stddef.h>

typedef enum _HttpVersion HttpVersion;

enum _HttpVersion {
    HTTP_VERSION_UNKNOWN,
    HTTP_VERSION_0_9,
    HTTP_VERSION_1_0,
    HTTP_VERSION_1_1,
    HTTP_VERSION_2_0,
    HTTP_VERSION_3_0
};

HttpVersion HttpVersion_FromString(const char* src, size_t src_size);

const char* HttpVersion_ToString(HttpVersion version);

#endif