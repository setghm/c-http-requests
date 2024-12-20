#ifndef _REQUESTER_STRUCTS_HTTP_RESPONSE_H
#define _REQUESTER_STRUCTS_HTTP_RESPONSE_H

#include "string_map.h"
#include "http_content.h"
#include "http_status.h"
#include "http_version.h"

#define HTTP_INVALID_RESPONSE ((HttpResponse*)0)

typedef struct _HttpResponse HttpResponse;

struct _HttpResponse {
    StringMap* headers;

    StreamContent* content;

    HttpVersion version;

    HttpStatus status;
};

HttpResponse* HttpResponse_New(void);

void HttpResponse_Delete(HttpResponse* res);

char* HttpResponse_StartLineToString(const HttpResponse* res);

char* HttpResponse_ToString(HttpResponse* res);

void HttpResponse_ParseStartLine(HttpResponse* res, const char* src, size_t src_size);

HttpResponse* HttpResponse_ParseNew(const char* src, size_t src_size);

#endif