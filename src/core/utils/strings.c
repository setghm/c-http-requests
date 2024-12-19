#include "strings.h"

#include <stdlib.h>
#include <string.h>

#include "pointers.h"

char* strclone(const char* src) {
    RETURN_NULL_IF_NULL(src);

    return strnclone(src, strlen(src));
}

char* strnclone(const char* src, size_t length) {
    RETURN_NULL_IF_NULL(src);
    RETURN_NULL_IF_ZERO(length);

    char* dest = (char*)calloc(length + 1, sizeof(char));

    if (dest != NULL) {
        memset(dest, 0, length + 1);

        strncpy(dest, src, length);
    }

    return dest;
}

void strreplace(const char** dest, const char* src) {
    RETURN_IF_NULL(src);
    RETURN_IF_NULL(dest);
    RETURN_IF_NULL(*dest);

    RELEASE_IF_NOT_NULL(*dest);

    *dest = strclone(src);
}

void strnreplace(const char** dest, const char* src, size_t length) {
    RETURN_IF_NULL(src);
    RETURN_IF_NULL(dest);
    RETURN_IF_NULL(*dest);
    RETURN_IF_ZERO(length);

    RELEASE_IF_NOT_NULL(*dest);

    *dest = strnclone(src, length);
}
