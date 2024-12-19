#ifndef _REQUESTER_STRUCTS_STRING_MAP_H
#define _REQUESTER_STRUCTS_STRING_MAP_H

#include <stddef.h>
#include <utils/utils.h>

typedef struct _StringPair StringPair;
typedef struct _StringMap StringMap;
typedef struct __StringMapItem _StringMapItem;

struct _StringPair {
    const char* key;
    const char* value;
};

struct _StringMap {
    _StringMapItem* _begin;
    _StringMapItem* _end;

    size_t length;
};

struct __StringMapItem {
    StringPair* _data;
    _StringMapItem* _next;
};

/*
    Creates a new key-value string pair without values.

    The key and the value must be NULL-terminated strings.
*/
StringPair* StringPair_New(void);

/*
    Creates a new key-value string pair with the provided values.

    The key and the value must be NULL-terminated strings.
*/
StringPair* StringPair_NewWith(const char* key, const char* value);

/*
    Releases an existing key-value string pair.
*/
void StringPair_Delete(StringPair* pair);

/*
    Creates a new empty string map.
*/
StringMap* StringMap_New(void);

/*
    Releases an existing string map and all its items.
*/
void StringMap_Delete(StringMap* sm);

/*
    Creates a new string map with the same content of the provider map.
*/
StringMap* StringMap_Clone(StringMap* sm);

/*
    Creates a new key-value string pair from the given key and value and adds
    it to the map.

    If the provided key already exists in the map, its value is overwritten.

    The key and the value must be NULL-terminated strings.
*/
boolean StringMap_Add(StringMap* sm, const char* key, const char* value);

/*
    Creates a new key-value string pair from the given key and value and adds
    it to the map only if the given key is not added already.

    The key and the value must be NULL-terminated strings.
*/
boolean StringMap_AddIfNotExists(StringMap* sm, const char* key, const char* value);

/*
    Adds an existing key-value string pair to the map.

    If any pair with the given key already exists in the map, the existing pair
    is released and the new one is added.
*/
boolean StringMap_AddExisting(StringMap* sm, StringPair* pair);

/*
    Adds all the existing NOT-NULL StringPair* values from the va-list.

    If any key already exists in the map, its value is overwritten.
*/
void StringMap_AddMany(StringMap* sm, size_t count, ...);

/*
    Adds all the values from the other map into the first map.

    If any key already exists in the map, its value is overwritten.
*/
void StringMap_AddFrom(StringMap* sm, StringMap* other);

/*
    Seeks for a key-value string pair in the map that owns the given key and
    removes it from the struct, then that pointer to a StringPair struct is returned.

    If the pair is not in the map NULL is returned.

    The key must be a NULL-terminated string.
*/
StringPair* StringMap_Pop(StringMap* sm, const char* key);

/*
    Seeks for a key-value string pair in the map that owns the given key, then
    the struct is removed from the map, next, the struct is released and true
    is returned.

    If the pair is not in the map false is returned.

    The key must be a NULL-terminated string.
*/
boolean StringMap_Remove(StringMap* sm, const char* key);

/*
    Returns the first key-value string pair with the same supplied key.

    If the key is not found, NULL is returned.

    The key must be a NULL-terminated string.
*/
StringPair* StringMap_Get(StringMap* sm, const char* key);

/*
    Returns the first key-value string pair with the same supplied value.

    If the value is not found, NULL is returned.

    The value must be a NULL-terminated string.
*/
StringPair* StringMap_GetByValue(StringMap* sm, const char* value);

/*
    Search a key-value pair in the given index.

    If the index is not valid, NULL is returned.
*/
StringPair* StringMap_GetAt(StringMap* sm, size_t index);

/*
    Creates a new string map with all the key-value string pairs that owns
    the given value.

    If no item is found, an empty map is returned.

    The value must be a NULL-terminated string.
*/
StringMap* StringMap_FilterByValue(StringMap* sm, const char* value);

#endif