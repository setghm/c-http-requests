#include "url_protocol.h"

#include <string.h>

// requester
#include "url_protocol_text.h"

URLProtocol URLProtocol_FromString(const char* src, const size_t src_size) {
    if (strncmp(src, URLProtocolText(URL_PROTOCOL_HTTP), src_size) == 0) {
        return URL_PROTOCOL_HTTP;
    }
    if (strncmp(src, URLProtocolText(URL_PROTOCOL_HTTPS), src_size) == 0) {
        return URL_PROTOCOL_HTTPS;
    }
    if (strncmp(src, URLProtocolText(URL_PROTOCOL_WS), src_size) == 0) {
        return URL_PROTOCOL_WS;
    }

    return URL_PROTOCOL_UNKNOWN;
}

const char* URLProtocol_ToString(const URLProtocol protocol) {
    switch (protocol) {
        case URL_PROTOCOL_HTTP:
            return URL_PROTOCOL_HTTP_TEXT;
        case URL_PROTOCOL_HTTPS:
            return URL_PROTOCOL_HTTPS_TEXT;
        case URL_PROTOCOL_WS:
            return URL_PROTOCOL_WS_TEXT;
        default: return "";
    }
}
