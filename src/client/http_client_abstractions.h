#ifndef _REQUESTER_CLIENT_HTTP_CLIENT_ABSTRACTIONS_H
#define _REQUESTER_CLIENT_HTTP_CLIENT_ABSTRACTIONS_H

#include <core/core.h>

HttpResponse* HttpClient_Get(const char* url);

HttpResponse* HttpClient_Post(const char* url, const char* data, MIMEType content_type);

HttpResponse* HttpClient_PostFile(const char* url, const char* file_name, MIMEType content_type);

HttpResponse* HttpClient_Put(const char* url, const char* data, MIMEType content_type);

HttpResponse* HttpClient_PutFile(const char* url, const char* file_name, MIMEType content_type);

HttpResponse* HttpClient_Delete(const char* url);

HttpResponse* HttpClient_Head(const char* url);

#endif
