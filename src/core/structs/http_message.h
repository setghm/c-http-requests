#ifndef _REQUESTER_STRUCTS_HTTP_MESSAGE_H
#define _REQUESTER_STRUCTS_HTTP_MESSAGE_H

#include "http_content.h"
#include "string_map.h"
#include "utils/utils.h"

#define BaseHttpMessage(derived) ((HttpMessage*)derived)

typedef struct _HttpMessage HttpMessage;

struct _HttpMessage {
    StringMap* headers;

    HttpContent* content;
};

#endif
