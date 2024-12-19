#include "http_method.h"

#include <string.h>

// requester
#include "http_method_text.h"

HttpMethod HttpMethod_FromString(const char* src, size_t src_size) {
    if (strncmp(src, HttpMethodText(HTTP_METHOD_GET), src_size) == 0) {
        return HTTP_METHOD_GET;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_HEAD), src_size) == 0) {
        return HTTP_METHOD_HEAD;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_POST), src_size) == 0) {
        return HTTP_METHOD_POST;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_PUT), src_size) == 0) {
        return HTTP_METHOD_PUT;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_PATCH), src_size) == 0) {
        return HTTP_METHOD_PATCH;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_DELETE), src_size) == 0) {
        return HTTP_METHOD_DELETE;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_OPTIONS), src_size) == 0) {
        return HTTP_METHOD_OPTIONS;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_TRACE), src_size) == 0) {
        return HTTP_METHOD_TRACE;
    }
    if (strncmp(src, HttpMethodText(HTTP_METHOD_CONNECT), src_size) == 0) {
        return HTTP_METHOD_CONNECT;
    }

    return HTTP_METHOD_GET;
}

const char* HttpMethod_ToString(HttpMethod method) {
    switch (method) {
        case HTTP_METHOD_GET:
            return HTTP_METHOD_GET_TEXT;
        case HTTP_METHOD_POST:
            return HTTP_METHOD_POST_TEXT;
        case HTTP_METHOD_PUT:
            return HTTP_METHOD_PUT_TEXT;
        case HTTP_METHOD_DELETE:
            return HTTP_METHOD_DELETE_TEXT;
        case HTTP_METHOD_PATCH:
            return HTTP_METHOD_PATCH_TEXT;
        case HTTP_METHOD_HEAD:
            return HTTP_METHOD_HEAD_TEXT;
        case HTTP_METHOD_OPTIONS:
            return HTTP_METHOD_OPTIONS_TEXT;
        case HTTP_METHOD_CONNECT:
            return HTTP_METHOD_CONNECT_TEXT;
        case HTTP_METHOD_TRACE:
            return HTTP_METHOD_TRACE_TEXT;
    }

    return "";
}