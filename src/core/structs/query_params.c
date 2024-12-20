#include "query_params.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// requester
#include <http_default.h>
#include <utils/utils.h>

StringMap* QueryParams_ParseNew(const char* src, const size_t src_size) {
    RETURN_NULL_IF_NULL(src);
    RETURN_NULL_IF_ZERO(src_size);

    StringMap* query_param = StringMap_New();
    const size_t len = strlen(src);

    const char *start, *end = src;

    do {
        start = end;

        /*
            Skip '&' and '?' characters before search the next '&' delimiter.
        */
        if (start[0] == '&' || start[0] == '?') {
            start++;
        }

        end = strstr(start, HTTP_DELIMITER_QUERY_PAIR);

        /*
            Separate the key-value pair in a different buffer.
        */
        const size_t buffer_size = end ? end - start : len - (size_t)(start - src);
        char* buffer = strnclone(start, buffer_size);

        StringPair* param = QueryParameter_ParseNew(buffer, buffer_size);

        free(buffer);

        /*
            Append the key value pair if it was parsed correctly.
        */
        if (param) {
            StringMap_AddExisting(query_param, param);
        }

    } while (end);

    return query_param;
}

StringPair* QueryParameter_ParseNew(const char* src, const size_t src_size) {
    RETURN_NULL_IF_NULL(src);
    RETURN_NULL_IF_ZERO(src_size);

    /*
        Get the position of the first '=' character.
    */
    const char* value_ptr = strstr(src, HTTP_DELIMITER_QUERY_VALUE);

    /*
        Skip malformed key-value query pairs.
    */
    if (value_ptr == NULL) {
        return NULL;
    }

    StringPair* param = StringPair_New();

    param->key = strnclone(src, (size_t)(value_ptr - src));

    /*
        Skip the '=' character.
    */
    value_ptr++;

    param->value = strclone(value_ptr);

    return param;
}

char* QueryParams_ToString(StringMap* query_params) {
    RETURN_NULL_IF_NULL(query_params);

    /*
        Calculate the length of the string.
    */
    size_t len = 1;  // NULL terminator
    size_t index = 0;

    for (index = 0; index < query_params->length; index++) {
        const StringPair* param = StringMap_GetAt(query_params, index);

        /*
            Add the length of the name, the length of the value and the character '='.
        */
        len += strlen(param->key) + strlen(param->value) + 1;

        /*
            Add the character '&' if this is not the last item.
        */
        if (index < query_params->length - 1) {
            len++;
        }
    }

    /*
        Alloc size for the string.
    */
    size_t i;
    char* str = malloc(len);

    if (str) {
        memset(str, 0, len);

        /*
            Dump the values.
        */
        for (index = 0, i = 0; index < query_params->length; index++) {
            const StringPair* param = StringMap_GetAt(query_params, index);

            if (index < query_params->length - 1) {
                i += sprintf((str + i), "%s=%s&", param->key, param->value);
            } else {
                i += sprintf((str + i), "%s=%s", param->key, param->value);
            }
        }
    }

    return str;
}
