#include "http_method.h"

#include <string.h>

// requester
#include "http_method_text.h"

#define CHECK_HTTP_METHOD_STRING(method)\
    if (strncmp(src, HttpMethodText(method), src_size) == 0) return method

HttpMethod HttpMethod_FromString(const char* src, const size_t src_size) {
    if (src == NULL || src_size == 0) {
        return HTTP_METHOD_GET;
    }

    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_GET);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_HEAD);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_POST);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_PUT);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_PATCH);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_DELETE);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_OPTIONS);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_TRACE);
    CHECK_HTTP_METHOD_STRING(HTTP_METHOD_CONNECT);

    return HTTP_METHOD_GET;
}

const char* HttpMethod_ToString(const HttpMethod method) {
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
        default: return "";
    }
}