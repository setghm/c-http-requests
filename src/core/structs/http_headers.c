#include "http_headers.h"

#include <string.h>
#include <stdio.h>

// requester
#include <http_default.h>
#include <utils/utils.h>

StringPair* HttpHeader_ParseNew(const char* src, const size_t src_size) {
    RETURN_NULL_IF_NULL(src);
    RETURN_NULL_IF_ZERO(src_size);

    /*
        Find the ':' delimiter.
    */
    const char* delimiter_pos = strstr(src, HTTP_DELIMITER_HEADER);

    if (delimiter_pos == NULL) {
        return NULL;
    }

    StringPair* header = StringPair_New();

    /*
        Get the header name.
    */
    const size_t name_len = (size_t)(delimiter_pos - src);

    header->key = strnclone(src, name_len);

    /*
        Get the header value.
    */
    const char* value_pos = delimiter_pos + 1;

    /*
        Trim the value from left.
    */
    while (*value_pos == ' ') value_pos++;

    header->value = strclone(value_pos);

    return header;
}

char* HttpHeaders_ToString(StringMap* headers) {
    RETURN_NULL_IF_NULL(headers);

    /*
        Calculate the length of the string.
    */
    size_t len = 1;  // NULL terminator
    size_t index = 0;

    for (index = 0; index < headers->length; index++) {
        const StringPair* header = StringMap_GetAt(headers, index);

        /*
            Add the length of the name, the length of the value and the string ": ".
        */
        len += strlen(header->key) + strlen(header->value) + 2;

        /*
            Add the separator "\r\n".
        */
        len += 2;
    }

    /*
        Alloc size for the string.
    */
    size_t i;
    char* str = (char*)malloc(len);

    if (str) {
        memset(str, 0, len);

        /*
            Dump the values.
        */
        for (index = 0, i = 0; index < headers->length; index++) {
            const StringPair* header = StringMap_GetAt(headers, index);

            i += sprintf((str + i), "%s: %s\r\n", header->key, header->value);
        }
    }

    return str;
}
