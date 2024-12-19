#ifndef _REQUESTER_STRUCTS_URL_PROTOCOL_TEXT_H
#define _REQUESTER_STRUCTS_URL_PROTOCOL_TEXT_H

#define _URLProtocolTextExpanded(name) name
#define URLProtocolText(proto) _URLProtocolTextExpanded(proto##_TEXT)

#define URL_PROTOCOL_UNKNOWN_TEXT ""
#define URL_PROTOCOL_HTTP_TEXT "http"
#define URL_PROTOCOL_HTTPS_TEXT "https"
#define URL_PROTOCOL_WS_TEXT "ws"

#endif