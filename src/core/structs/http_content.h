#ifndef _REQUESTER_STRUCTS_HTTP_CONTENT_H
#define _REQUESTER_STRUCTS_HTTP_CONTENT_H

#include <stddef.h>

// requester
#include "mime_type.h"
#include <network/network.h>
#include <utils/utils.h>

#define HTTP_NO_CONTENT ((HttpContent*)0)
#define STREAM_CONTENT_SEND_ALL -1

#define BaseHttpContent(derived) ((HttpContent*)derived)

typedef enum __HttpContentKind _HttpContentKind;
typedef struct _HttpContent HttpContent;
typedef struct _ByteBufferContent ByteBufferContent;
typedef struct _FileContent FileContent;
typedef struct _StreamContent StreamContent;

enum __HttpContentKind {
    _HTTP_CONTENT_KIND_BYTE_BUFFER,
    _HTTP_CONTENT_KIND_FILE,
    _HTTP_CONTENT_KIND_STREAM
};

struct _HttpContent {
    /*
        READ-ONLY.
        
        The subtype of HttpContent content.
    */
    _HttpContentKind _kind;

    /*
        READ-ONLY.

        The number of bytes to send or receive.

        When you create a StreamContent to be sent, this value is not used, but you may
        consider setting the Content-Length header in the request if the remote server
        requires that information.
    */
    size_t content_length;

    /*
        READ-WRITE.

        The MIME type of the content.
        
        This value should not be overridden if the content structure belongs to an
        HttpResponse structure since it contains information about the data to be received.
    */
    MIMEType content_type;
};

struct _ByteBufferContent {
    HttpContent _base;

    byte* _buffer;
};

struct _FileContent {
    HttpContent _base;

    char* _file_name;
};

struct _StreamContent {
    HttpContent _base;

    ClientSocket* _source;
};

/*
    Creates a new ByteBufferContent struct by copying the number of bytes specified in
    "buffer_size" from the provided buffer into a new buffer.

    The content-length value is set equal as "buffer_size" value.
*/
ByteBufferContent* ByteBufferContent_New(const byte* buffer, size_t buffer_size);

void ByteBufferContent_Delete(ByteBufferContent* content);

size_t ByteBufferContent_Send(ByteBufferContent* content, ClientSocket* cs);

/*
    Creates a new FileContent struct and attempts to open the specified file in binary read
    mode to get its byte size and set the content-length to that value
    
    After creating the structure the file is closed until the next call to FileContent_Send.
*/
FileContent* FileContent_New(const char* file_name);

void FileContent_Delete(FileContent* content);

size_t FileContent_Send(const FileContent* content, ClientSocket* cs);

/*
    Creates a new StreamContent struct that reads data from a client socket.
*/
StreamContent* StreamContent_New(ClientSocket* source);

void StreamContent_Delete(StreamContent* content);

size_t StreamContent_Read(const StreamContent* content, const byte* buffer, size_t buffer_size);

char* StreamContent_ReadAsString(const StreamContent* content);

size_t StreamContent_ReadToFile(const StreamContent* content, const char* file_name);

/*
    Reads the content from the source and writes it to the provided socket, this function
    works like a pipe.

    If "max_bytes_to_send" is greater than zero, no more bytes than specified will be sent.

    If "max_bytes_to send" is less than zero, the function will try to read the entire
    source content and will write it to the provided socket until the end.

    If "max_bytes_to_send" is zero, no content will be send.
*/
size_t StreamContent_Send(StreamContent* content, ClientSocket* cs, u64 max_bytes_to_send);

size_t HttpContent_Send(HttpContent* content, ClientSocket* cs);

void HttpContent_Delete(HttpContent* content);

#endif