#ifndef _REQUESTER_STRUCTS_QUERY_PARAMS_H
#define _REQUESTER_STRUCTS_QUERY_PARAMS_H

#include <stddef.h>

#include "string_map.h"

StringMap* QueryParams_ParseNew(const char* src, size_t src_size);

StringPair* QueryParameter_ParseNew(const char* src, size_t src_size);

char* QueryParams_ToString(StringMap* query_params);

#endif