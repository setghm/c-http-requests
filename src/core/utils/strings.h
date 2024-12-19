#ifndef _REQUESTER_UTILS_STRINGS_H
#define _REQUESTER_UTILS_STRINGS_H

#include <stddef.h>

/*
    Creates a copy of src and returns the pointer to the new buffer.

    Includes the null-terminator character on the new buffer.
*/
char* strclone(const char* src);

/*
    Creates a copy of N bytes from src and returns the pointer to the new buffer.

    Includes the null-terminator character on the new buffer.
*/
char* strnclone(const char* src, size_t length);

/*
    Copies src into a new buffer and sets that pointer to *dest, if *dest
    is not null, it is released first.

    Includes the null-terminator character on the new buffer.
*/
void strreplace(const char** dest, const char* src);

/*
    Copies N bytes from src into a new buffer and sets that pointer to *dest,
    if *dest is not null, it is released first.

    Includes the null-terminator character on the new buffer.
*/
void strnreplace(const char** dest, const char* src, size_t length);

#endif