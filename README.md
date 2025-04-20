HTTP requests library for the C programming language

Features

- Response handling in streams
- Support for HTTP and HTTPS (depending on OpenSSL)
- Implementation for a StringMap struct used in HTTP headers list and URL parameters
- URL parsing
- Shorthand functions

## Example

Getting your external IP address

[get_external_ip_address.c](examples\get_external_ip_address.c)
```c
#include <requester.h> // <-- This library
#include <stdio.h>

int main(void) {
    /*
        Initialize library.
    */
    HttpClient_Init();

    /*
        Perform the request.
    */
    HttpResponse* res = HttpClient_Get("https://ifconfig.me/ip");

    if (res == HTTP_INVALID_RESPONSE) {
        puts("Cannot get your external IP address.");
        return -1;
    }

    char* ip = StreamContent_ReadAsString(res->content);

    if (ip != NULL) {
        puts(ip);

        free(ip);
    }

    /*
        Cleanup resources.
    */
    HttpResponse_Delete(res);
    HttpClient_Cleanup();

    return 0;
}
```

## HTTP Version support

- HTTP 0.9 (untested)
- HTTP 1.1 (tested)
- HTTP 2 and 3 (untested)

## Shorthand functions

Use these functions to avoid creating a HttpRequest struct explicitly.

[http_client_abstractions.h](.\src\client\http_client_abstractions.h)
```c
HttpResponse* HttpClient_Get(const char* url);
HttpResponse* HttpClient_Post(const char* url, const char* data, MIMEType content_type);
HttpResponse* HttpClient_PostFile(const char* url, const char* file_name, MIMEType content_type);
HttpResponse* HttpClient_Put(const char* url, const char* data, MIMEType content_type);
HttpResponse* HttpClient_PutFile(const char* url, const char* file_name, MIMEType content_type);
HttpResponse* HttpClient_Delete(const char* url);
HttpResponse* HttpClient_Head(const char* url);
```

## Request and response structures

[http_request.h](.\src\core\structs\http_request.h)
```c
struct _HttpResponse {
    StringMap* headers;
    StreamContent* content;
    HttpVersion version;
    HttpStatus status;
};
```

[http_response.h](.\src\core\structs\http_response.h)
```c
struct _HttpRequest {
    StringMap* headers;
    HttpContent* content;
    URL* url;
    HttpMethod method;
    HttpVersion version;
};
```
