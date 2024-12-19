#include "int_string.h"
#include <stdio.h>

i64 parse_i64(const char* string) {
	i64 value = 0;
	sscanf(string, "%lld", &value);
	return value;
}

i32 parse_i32(const char* string) {
	i32 value = 0;
	sscanf(string, "%ld", &value);
	return value;
}

i16 parse_i16(const char* string) {
	i16 value = 0;
	sscanf(string, "%hd", &value);
	return value;
}

u64 parse_u64(const char* string) {
	u64 value = 0;
	sscanf(string, "%llu", &value);
	return value;
}

u32 parse_u32(const char* string) {
	u32 value = 0;
	sscanf(string, "%lu", &value);
	return value;
}

u16 parse_u16(const char* string) {
	u16 value = 0;
	sscanf(string, "%hu", &value);
	return value;
}
