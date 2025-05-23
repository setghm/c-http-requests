#include "http_client.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// requester
#include <helpers/helpers.h>

#include "config.h"

boolean HttpClient_Init(void) {
    SecureClientLayers_Init();

    return true;
}

void HttpClient_Cleanup(void) {
    SecureClientLayers_Cleanup();
}

HttpResponse* HttpClient_Send(HttpRequest* req) {
    RETURN_NULL_IF_NULL(req);
    RETURN_NULL_IF_NULL(req->url);
    RETURN_NULL_IF_NULL(req->url->host);
    RETURN_NULL_IF_NULL(req->url->resource);

    /*
        Attempt to open the connection to host.
    */
    ClientSocket* client = ClientSocket_Open(req->url->host, req->url->port);

    if (req->url->protocol != URL_PROTOCOL_HTTP) {
        ClientSocket_MakeSecure(client);
    }

    RETURN_NULL_IF_NULL(client);

    /*
        Set http request needed headers.
    */
    StringMap_AddIfNotExists(req->headers, "Host", req->url->host);
    StringMap_AddIfNotExists(req->headers, "Accept", DEFAULT_ACCEPT);
    StringMap_AddIfNotExists(req->headers, "Accept-Encoding", DEFAULT_ACCEPT_ENCODING);
    StringMap_AddIfNotExists(req->headers, "Connection", DEFAULT_CONNECTION);
    StringMap_AddIfNotExists(req->headers, "User-Agent", UA_DEFAULT);

    if (req->content != HTTP_NO_CONTENT) {
        /*
            Add content length header.
        */
        char content_length[20] = {0};
        sprintf(content_length, "%llu", req->content->content_length);

        StringMap_AddIfNotExists(req->headers, "Content-Length", content_length);

        /*
            Add content type header.
        */
        const char* content_type = MIMEType_ToString(req->content->content_type);

        StringMap_AddIfNotExists(req->headers, "Content-Type", content_type);
    }

    /*
        Get the HttpRequest as string and send it.
    */
    char* req_str = HttpRequest_ToString(req);

    if (req_str != NULL) {
        const size_t req_str_size = strlen(req_str);

        size_t bytes_written = ClientSocket_Write(client, (const byte*)req_str, req_str_size);

#ifdef DEBUG
        fprintf(stderr, "[DEBUG][HttpClient] %d bytes sent of request head.\n", bytes_written);
#endif

        free(req_str);
    }

    /*
        Send the request content if it has.
    */
    if (req->content != HTTP_NO_CONTENT) {
        HttpContent_Send(req->content, client);
    }

    /*
        Close this socket peer.
    */
    //ClientSocket_FinishWriting(client);

    /*
        Get the response.

        Read byte by byte until the payload delimiter.

        Then, parse the http response.
    */
    size_t buffer_size = CHUNK_SIZE_RESPONSE_READ;
    size_t total_bytes_read = 0, bytes_read;

    char* buffer = (char*)malloc(buffer_size);

    if (buffer) {
        memset(buffer, 0, buffer_size);

        byte last_byte = 0;
        byte last_bytes[4] = {0};

        do {
            bytes_read = ClientSocket_Read(client, &last_byte, 1);

            BREAK_IF_ZERO(bytes_read);

            last_bytes[0] = last_bytes[1];
            last_bytes[1] = last_bytes[2];
            last_bytes[2] = last_bytes[3];
            last_bytes[3] = last_byte;

            buffer[total_bytes_read] = (char)last_byte;

            total_bytes_read++;

            /*
                Reallocate the buffer if more space is needed.
            */
            if (total_bytes_read > buffer_size - 1) {
                const size_t new_buffer_size = buffer_size + CHUNK_SIZE_RESPONSE_READ;

                char* new_buffer = (char*)realloc(buffer, new_buffer_size);

                /*
                    Break if out of memory.
                */
                BREAK_IF_NULL(new_buffer);

                buffer = new_buffer;

                memset(buffer + buffer_size, 0, buffer_size);

                buffer_size = new_buffer_size;
            }

        } while (!(
            last_bytes[0] == '\r' && last_bytes[1] == '\n' &&
            last_bytes[2] == '\r' && last_bytes[3] == '\n'));
    }

    HttpResponse* res = HttpResponse_ParseNew(buffer, total_bytes_read);

    if (res == NULL) {
        ClientSocket_Close(client);
        return NULL;
    }

    /*
        Create a new StreamContent for the response, and set the client socket as source.

        The socket remains opened until the response structure is deleted.
    */
    res->content = StreamContent_New(client);

    /*
        Find the content length header and set it to StreamContent.
    */
    const StringPair* header = StringMap_Get(res->headers, "Content-Length");

    if (header != NULL) {
        BaseHttpContent(res->content)->content_length = parse_u64(header->value);
    }

    return res;
}
