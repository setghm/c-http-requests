#ifndef _REQUESTER_STRUCTS_URL_H
#define _REQUESTER_STRUCTS_URL_H

#include "string_map.h"
#include "url_protocol.h"
#include "utils/utils.h"

typedef struct _URL URL;

struct _URL {
    URLProtocol protocol;

    char* user;

    char* password;

    u16 port;

    char* host;

    char* resource;

    StringMap* query_params;

    char* fragment;
};

/*
    Creates a new URL struct with an empty map for query_params.
*/
URL* URL_New(void);

void URL_Delete(URL* url);

void URL_ResourceParse(URL* url, const char* src);

URL* URL_ParseNew(const char* src);

char* URL_ToString(URL* url);

char* URL_ResourceToString(const URL* url);

#endif