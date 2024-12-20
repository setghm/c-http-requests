#ifndef _REQUESTER_STRUCTS_HTTP_REQUEST_H
#define _REQUESTER_STRUCTS_HTTP_REQUEST_H

#include "http_message.h"
#include "http_method.h"
#include "http_version.h"
#include "url.h"

typedef struct _HttpRequest HttpRequest;

struct _HttpRequest {
    /*
        The base structure.
    */
    HttpMessage _base;

    URL* url;

    HttpMethod method;

    HttpVersion version;
};

HttpRequest* HttpRequest_New(void);

void HttpRequest_Delete(HttpRequest* req);

char* HttpRequest_StartLineToString(HttpRequest* req);

char* HttpRequest_ToString(HttpRequest* req);

void HttpRequest_ParseStartLine(HttpRequest* req, const char* src, size_t src_size);

HttpRequest* HttpRequest_ParseNew(const char* src, size_t src_size);

#endif
