#ifndef _REQUESTER_STRUCTS_HTTP_HEADERS_H
#define _REQUESTER_STRUCTS_HTTP_HEADERS_H

#include <stddef.h>

#include "string_map.h"

StringPair* HttpHeader_ParseNew(const char* src, size_t src_size);

char* HttpHeaders_ToString(StringMap* headers);

#endif