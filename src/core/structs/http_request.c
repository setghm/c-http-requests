#include "http_request.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// requester
#include "http_headers.h"
#include <config.h>
#include <http_default.h>
#include <utils/utils.h>

HttpRequest* HttpRequest_New(void) {
    HttpRequest* req = (HttpRequest*)malloc(sizeof(HttpRequest));

    if (req) {
        memset(req, 0, sizeof(HttpRequest));

        req->method = HTTP_DEFAULT_REQUEST_METHOD;
        req->version = HTTP_DEFAULT_REQUEST_VERSION;

        req->headers = StringMap_New();
    }

    return req;
}

void HttpRequest_Delete(HttpRequest* req) {
    RETURN_IF_NULL(req);

    URL_Delete(req->url);
    StringMap_Delete(req->headers);
    HttpContent_Delete(req->content);

    free(req);
}

char* HttpRequest_StartLineToString(HttpRequest* req) {
    RETURN_NULL_IF_NULL(req);

    /*
        Calculate the length of the string.
    */
    size_t len = 3;  // NULL character + 2 whitespaces

    const char* method_str = HttpMethod_ToString(req->method);

    len += strlen(method_str);

    char* resource_str = URL_ResourceToString(req->url);

    if (resource_str) {
        len += strlen(resource_str);
    }

    const char* version_str = HttpVersion_ToString(req->version);

    len += strlen(version_str);

    /*
        Alloc the string.
    */
    char* str = (char*)malloc(len);

    if (str) {
        memset(str, 0, len);

        /*
            Dump values.
        */
        sprintf(str, "%s %s %s", method_str, resource_str, version_str);
    }

    /*
        Relase strings.
    */
    RELEASE_IF_NOT_NULL(resource_str);

    return str;
}

char* HttpRequest_ToString(HttpRequest* req) {
    RETURN_NULL_IF_NULL(req);

    /*
        Calculate the length of the string.
    */
    size_t len = 1;

    /*
        Convert the start line to string.
    */
    char* start_str = HttpRequest_StartLineToString(req);

    if (start_str) {
        /*
            Append the "\r\n" terminator.
        */
        len += strlen(start_str) + 2;
    }

    /*
        Convert the headers to string.
    */
    char* headers_str = HttpHeaders_ToString(req->headers);

    if (headers_str) {
        len += strlen(headers_str);
    }

    /*
        Append the "\r\n" payload separator (or request end).
    */
   len += 2;

    /*
        Allocate the string.
    */
    char* str = (char*)malloc(len);

    if (str) {
        memset(str, 0, len);

        /*
            Dump values.
        */
        if (headers_str) {
            sprintf(str, "%s\r\n%s\r\n", start_str, headers_str);
        } else {
            sprintf(str, "%s\r\n\r\n", start_str);
        }
    }

    /*
        Release the strings.
    */
    RELEASE_IF_NOT_NULL(start_str);
    RELEASE_IF_NOT_NULL(headers_str);

    return str;
}

void HttpRequest_ParseStartLine(HttpRequest* req, const char* buffer, size_t buffer_size) {
    RETURN_IF_NULL(req);
    RETURN_IF_NULL(buffer);
    RETURN_IF_ZERO(buffer_size);

    char arg1[20] = {0};
    char arg3[20] = {0};
    char* arg2 = (char*)calloc(2048, sizeof(char));

    int result = sscanf(buffer, "%s %s %s", arg1, arg2, arg3);

    if (result >= 2) {
        req->method = HttpMethod_FromString(arg1, 20);

        URL_ResourceParse(req->url, arg2);

        req->version = (result == 3) ? HttpVersion_FromString(arg3, 20) : HTTP_VERSION_0_9;
    }

    free(arg2);
}

HttpRequest* HttpRequest_ParseNew(const char* src, size_t src_size) {
    RETURN_NULL_IF_NULL(src);
    RETURN_NULL_IF_ZERO(src_size);

    /*
        Allocate a new request struct.
    */
    HttpRequest* req = HttpRequest_New();

    /*
        Read line by line.
    */
    const char* start = src;
    const char* end = strstr(start, HTTP_DELIMITER_MESSAGE_LINE);

    /*
         Parse request line.
    */
    HttpRequest_ParseStartLine(req, src, end != NULL ? (size_t)(end - src) : src_size);

    /*
        Finish if using HTTP/0.9
    */
    if (req->version == HTTP_VERSION_0_9) {
        return req;
    }

    /*
        Check if the string has the payload delimiter.
    */
    char* payload_start = strstr(src, HTTP_DELIMITER_MESSAGE_PAYLOAD);

    /*
        Parse request headers.
    */
    while (end && end != payload_start) {
        start = end + 2;
        end = strstr(start, HTTP_DELIMITER_MESSAGE_LINE);

        const size_t buffer_len = end ? (size_t)(end - start) : src_size - (size_t)(start - src);

        if (buffer_len != 0) {
            char* buffer = strnclone(start, buffer_len);

            StringPair* header = HttpHeader_ParseNew(buffer, buffer_len);

            free(buffer);

            if (header != NULL) {
                StringMap_AddExisting(req->headers, header);
            }
        }
    }

    return req;
}
