#ifndef _REQUESTER_UTILS_FLOAT_STRING_H
#define _REQUESTER_UTILS_FLOAT_STRING_H

#include "data_types_aliases.h"

#define parse_float parse_f32
#define parse_double parse_f64

f64 parse_f64(const char* string);
f32 parse_f32(const char* string);

#endif