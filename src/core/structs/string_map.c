#include "string_map.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// requester
#include <utils/utils.h>

StringPair* StringPair_New(void) {
    StringPair* pair = (StringPair*)malloc(sizeof(StringPair));

    if (pair) {
        memset(pair, 0, sizeof(StringPair));
    }

    return pair;
}

StringPair* StringPair_NewWith(const char* key, const char* value) {
    StringPair* pair = (StringPair*)malloc(sizeof(StringPair));

    if (pair) {
        memset(pair, 0, sizeof(StringPair));

        pair->key = strclone(key);
        pair->value = strclone(value);
    }

    return pair;
}

void StringPair_Delete(StringPair* pair) {
    RETURN_IF_NULL(pair);

    RELEASE_IF_NOT_NULL(pair->key);
    RELEASE_IF_NOT_NULL(pair->value);
    free(pair);
}

StringMap* StringMap_New(void) {
    StringMap* sm = (StringMap*)malloc(sizeof(StringMap));

    if (sm) {
        memset(sm, 0, sizeof(StringMap));
    }

    return sm;
}

void StringMap_Delete(StringMap* sm) {
    RETURN_IF_NULL(sm);

    _StringMapItem* item = sm->_begin;

    while (item != NULL) {
        _StringMapItem* next = item->_next;

        StringPair_Delete(item->_data);
        free(item);

        item = next;
    }

    sm->length = 0;
    free(sm);
}

StringMap* StringMap_Clone(StringMap* sm) {
    RETURN_NULL_IF_NULL(sm);

    StringMap* new_sm = StringMap_New();

    _StringMapItem* item = sm->_begin;

    while (item != NULL) {
        StringMap_Add(new_sm, item->_data->key, item->_data->value);

        item = item->_next;
    }

    return new_sm;
}

boolean StringMap_Add(StringMap* sm, const char* key, const char* value) {
    RETURN_ZERO_IF_NULL(sm);
    RETURN_ZERO_IF_NULL(key);
    RETURN_ZERO_IF_NULL(value);

    /*
        Check if there aren't a existing item with the given key.
    */
    StringPair* existing = StringMap_Get(sm, key);

    if (existing) {
        existing->value = value;
    } else {
        StringPair* pair = StringPair_New();
        pair->key = strclone(key);
        pair->value = strclone(value);

        StringMap_AddExisting(sm, pair);
    }
}

boolean StringMap_AddIfNotExists(StringMap* sm, const char* key, const char* value) {
    RETURN_ZERO_IF_NULL(sm);
    RETURN_ZERO_IF_NULL(key);
    RETURN_ZERO_IF_NULL(value);

    /*
        Check if there aren't a existing item with the given key.
    */
    StringPair* existing = StringMap_Get(sm, key);

    if (existing == NULL) {
        StringPair* pair = StringPair_New();
        pair->key = strclone(key);
        pair->value = strclone(value);

        StringMap_AddExisting(sm, pair);
    }
}

boolean StringMap_AddExisting(StringMap* sm, StringPair* pair) {
    RETURN_ZERO_IF_NULL(sm);
    RETURN_ZERO_IF_NULL(pair);
    RETURN_ZERO_IF_NULL(pair->key);
    RETURN_ZERO_IF_NULL(pair->value);

    /*
        Check if there aren't a existing item with the given key.
    */
    StringPair* existing = StringMap_Get(sm, pair->key);

    if (existing) {
        StringMap_Remove(sm, existing);

        StringPair_Delete(existing);
    }

    _StringMapItem* item = (StringMap*)malloc(sizeof(_StringMapItem));

    if (!item) {
        return;
    }

    memset(item, 0, sizeof(_StringMapItem));

    item->_data = pair;

    if (sm->_begin == NULL) {
        sm->_begin = item;
    }

    if (sm->_end != NULL) {
        sm->_end->_next = item;
    }

    sm->_end = item;
    sm->length++;
}

void StringMap_AddMany(StringMap* sm, size_t count, ...) {
    RETURN_IF_NULL(sm);
    RETURN_IF_ZERO(count);

    /*
        Iterate over the variable arguments.
    */
    va_list args;

    va_start(args, count);

    while (count-- > 0) {
        StringMap_AddExisting(sm, va_arg(args, StringPair*));
    }

    va_end(args);
}

void StringMap_AddFrom(StringMap* sm, StringMap* other) {
    RETURN_IF_NULL(sm);
    RETURN_IF_NULL(other);

    _StringMapItem* item = other->_begin;

    while (item) {
        CONTINUE_IF_NULL(item->_data);

        /*
            Overwrite entries with same keys.
        */
        StringPair* pair = StringMap_Get(sm, item->_data->key);

        if (pair) {
            RELEASE_IF_NOT_NULL(pair->value);
            pair->value = strclone(item->_data->value);
        } else {
            StringMap_Add(sm, item->_data->key, item->_data->value);
        }

        item = item->_next;
    }
}

StringPair* StringMap_Pop(StringMap* sm, const char* key) {
    RETURN_NULL_IF_ZERO(sm->length);
    RETURN_NULL_IF_NULL(key);

    _StringMapItem* current = sm->_begin;
    _StringMapItem* prev = NULL;

    while (current) {
        _StringMapItem* next = current->_next;

        if (current->_data->key == key) {
            if (prev != NULL) {
                prev->_next = next;
            } else {
                sm->_begin = next;
            }

            sm->_end = next;
            sm->length--;

            StringPair* pair = current->_data;

            free(current);

            return pair;
        }

        prev = current;
        current = next;
    }

    return NULL;
}

boolean StringMap_Remove(StringMap* sm, const char* key) {
    RETURN_ZERO_IF_NULL(sm);
    RETURN_ZERO_IF_NULL(key);

    StringPair* pair = StringMap_Pop(sm, key);

    if (pair) {
        StringPair_Delete(pair);

        return true;
    }

    return false;
}

StringPair* StringMap_Get(StringMap* sm, const char* key) {
    RETURN_NULL_IF_NULL(sm);
    RETURN_NULL_IF_NULL(key);
    RETURN_NULL_IF_ZERO(sm->length);

    _StringMapItem* item = sm->_begin;

    /*
        Get the target length.
    */
    size_t tlen = strlen(key);

    while (item != NULL) {
        size_t len = strlen(item->_data->key);

        if (strncmp(item->_data->key, key, MIN(len, tlen)) == 0) {
            return item->_data;
        }

        item = item->_next;
    }

    return NULL;
}

StringPair* StringMap_GetByValue(StringMap* sm, const char* value) {
    RETURN_NULL_IF_ZERO(sm->length);
    RETURN_NULL_IF_NULL(value);

    _StringMapItem* item = sm->_begin;

    /*
        Get the target length.
    */
    size_t tlen = strlen(value);

    while (item != NULL) {
        size_t len = strlen(item->_data->value);

        if (strncmp(item->_data->value, value, MIN(len, tlen)) == 0) {
            return item->_data;
        }

        item = item->_next;
    }

    return NULL;
}

StringPair* StringMap_GetAt(StringMap* sm, size_t index) {
    RETURN_NULL_IF_ZERO(sm->length);

    size_t current_index = 0;
    _StringMapItem* item = sm->_begin;

    while (item) {
        if (current_index++ == index) {
            return item->_data;
        }

        item = item->_next;
    }

    return NULL;
}

StringMap* StringMap_FilterByValue(StringMap* sm, const char* value) {
    RETURN_NULL_IF_NULL(sm);
    RETURN_NULL_IF_NULL(value);

    StringMap* filtered_sm = StringMap_New();
    _StringMapItem* item = sm->_begin;

    /*
        Get the target length.
    */
    size_t tlen = strlen(value);

    while (item) {
        size_t len = strlen(item->_data->value);

        if (strncmp(item->_data->value, value, MIN(len, tlen)) == 0) {
            StringMap_AddExisting(filtered_sm, item->_data);
        }

        item = item->_next;
    }

    if (filtered_sm->length == 0) {
        StringMap_Delete(filtered_sm);
        return NULL;
    }

    return filtered_sm;
}
