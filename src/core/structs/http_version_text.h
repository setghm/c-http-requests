#ifndef _REQUESTER_STRUCTS_HTTP_VERSION_TEXT_H
#define _REQUESTER_STRUCTS_HTTP_VERSION_TEXT_H

#define _HttpVersionTextExpanded(name) name
#define HttpVersionText(code) _HttpVersionTextExpanded(code##_TEXT)

#define HTTP_VERSION_0_9_TEXT "HTTP/0.9"
#define HTTP_VERSION_1_0_TEXT "HTTP/1.0"
#define HTTP_VERSION_1_1_TEXT "HTTP/1.1"
#define HTTP_VERSION_2_0_TEXT "HTTP/2"
#define HTTP_VERSION_3_0_TEXT "HTTP/3"
#define HTTP_VERSION_UNKNOWN_TEXT ""

#endif