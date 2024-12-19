#ifndef _REQUESTER_UTILS_INT_STRING_H
#define _REQUESTER_UTILS_INT_STRING_H

#include "data_types_aliases.h"

#define I64_MAX_CIPHERS 21
#define I32_MAX_CIPHERS 11
#define I16_MAX_CIPHERS 6
#define U64_MAX_CIPHERS 20
#define U32_MAX_CIPHERS 10
#define U16_MAX_CIPHERS 5

#define parse_int parse_i32

i64 parse_i64(const char* string);
i32 parse_i32(const char* string);
i16 parse_i16(const char* string);
u64 parse_u64(const char* string);
u32 parse_u32(const char* string);
u16 parse_u16(const char* string);

#endif