#ifndef _REQUESTER_CLIENT_HTTP_CLIENT_H
#define _REQUESTER_CLIENT_HTTP_CLIENT_H

#include <core/core.h>

boolean HttpClient_Init(void);

void HttpClient_Cleanup(void);

HttpResponse* HttpClient_Send(HttpRequest* req);

#endif