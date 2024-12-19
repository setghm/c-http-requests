#include "float_string.h"
#include <stdio.h>

f64 parse_f64(const char* string) {
	f64 value = 0;
	
	sscanf(string, "%lf", &value);
	
	return value;
}

f32 parse_f32(const char* string) {
	f32 value = 0;

	sscanf(string, "%f", &value);

	return value;
}