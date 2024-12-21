#include "http_client_abstractions.h"

// requests
#include "http_client.h"

HttpResponse* HttpClient_Get(const char* url) {
    HttpRequest* request = HttpRequest_New();

    request->url = URL_ParseNew(url);

    HttpResponse* response = HttpClient_Send(request);

    HttpRequest_Delete(request);

    return response;
}

HttpResponse* HttpClient_Post(const char* url, const char* data, const size_t data_length) {
    HttpRequest* request = HttpRequest_New();

    request->url = URL_ParseNew(url);

    request->content = (HttpContent*)ByteBufferContent_New((const byte*)data, data_length);

    request->method = HTTP_METHOD_POST;

    HttpResponse* response = HttpClient_Send(request);

    HttpRequest_Delete(request);

    return response;
}

HttpResponse* HttpClient_PostFile(const char* url, const char* file_name) {
    HttpRequest* request = HttpRequest_New();

    request->url = URL_ParseNew(url);

    request->content = (HttpContent*)FileContent_New(file_name);

    request->method = HTTP_METHOD_POST;

    HttpResponse* response = HttpClient_Send(request);

    HttpRequest_Delete(request);

    return response;
}

HttpResponse* HttpClient_Put(const char* url, const char* data, const size_t data_length) {
    HttpRequest* request = HttpRequest_New();

    request->url = URL_ParseNew(url);

    request->content = (HttpContent*)ByteBufferContent_New((const byte*)data, data_length);

    request->method = HTTP_METHOD_PUT;

    HttpResponse* response = HttpClient_Send(request);

    HttpRequest_Delete(request);

    return response;
}

HttpResponse* HttpClient_PutFile(const char* url, const char* file_name) {
    HttpRequest* request = HttpRequest_New();

    request->url = URL_ParseNew(url);

    request->content = (HttpContent*)FileContent_New(file_name);

    request->method = HTTP_METHOD_PUT;

    HttpResponse* response = HttpClient_Send(request);

    HttpRequest_Delete(request);

    return response;
}

HttpResponse* HttpClient_Delete(const char* url) {
    HttpRequest* request = HttpRequest_New();

    request->url = URL_ParseNew(url);

    request->method = HTTP_METHOD_DELETE;

    HttpResponse* response = HttpClient_Send(request);

    HttpRequest_Delete(request);

    return response;
}

HttpResponse* HttpClient_Head(const char* url) {
    HttpRequest* request = HttpRequest_New();

    request->url = URL_ParseNew(url);

    request->method = HTTP_METHOD_HEAD;

    HttpResponse* response = HttpClient_Send(request);

    HttpRequest_Delete(request);

    return response;
}
