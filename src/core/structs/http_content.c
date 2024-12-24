#include "http_content.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// requester
#include <config.h>
#include <utils/utils.h>

ByteBufferContent* ByteBufferContent_New(const byte* buffer, const size_t buffer_size) {
    ByteBufferContent* content = (ByteBufferContent*)malloc(sizeof(ByteBufferContent));

    if (content) {
        memset(content, 0, sizeof(ByteBufferContent));

        BaseHttpContent(content)->_kind = _HTTP_CONTENT_KIND_BYTE_BUFFER;
        BaseHttpContent(content)->content_length = buffer_size;

        content->_buffer = malloc(buffer_size);
        memset(content->_buffer, 0, buffer_size);
        memcpy(content->_buffer, buffer, buffer_size);
    }

    return content;
}

void ByteBufferContent_Delete(ByteBufferContent* content) {
    RETURN_IF_NULL(content);

    RELEASE_IF_NOT_NULL(content->_buffer);

    free(content);
}

size_t ByteBufferContent_Send(ByteBufferContent* content, ClientSocket* cs) {
    RETURN_ZERO_IF_NULL(content);
    RETURN_ZERO_IF_NULL(cs);

    return ClientSocket_Write(cs, content->_buffer, BaseHttpContent(content)->content_length);
}

FileContent* FileContent_New(const char* file_name) {
    FileContent* content = malloc(sizeof(FileContent));

    if (content) {
        memset(content, 0, sizeof(FileContent));

        BaseHttpContent(content)->_kind = _HTTP_CONTENT_KIND_FILE;

        content->_file_name = strclone(file_name);

        /*
            Attempt to open the file in binary read mode to get its size.
        */
        FILE* file = fopen(file_name, "rb");

        if (file) {
            fseek(file, 0, SEEK_END);
            
            BaseHttpContent(content)->content_length = ftell(file);

            fclose(file);
        }
    }

    return content;
}

void FileContent_Delete(FileContent* content) {
    RETURN_IF_NULL(content);

    RELEASE_IF_NOT_NULL(content->_file_name);

    free(content);
}

size_t FileContent_Send(const FileContent* content, ClientSocket* cs) {
    RETURN_ZERO_IF_NULL(content);
    RETURN_ZERO_IF_NULL(cs);

    /*
        Attempt to open the file in binary read mode.
    */
    FILE* file = fopen(content->_file_name, "rb");

    RETURN_ZERO_IF_NULL(file);

    /*
        Send the whole file.
    */
    size_t total_bytes_sent = 0, bytes_sent, bytes_read;
    byte buffer[CHUNK_SIZE_FILE_CONTENT_SEND] = { 0 };

    do {
        bytes_read = fread(buffer, 1, CHUNK_SIZE_FILE_CONTENT_SEND, file);

        bytes_sent = ClientSocket_Write(cs, buffer, bytes_read);

        total_bytes_sent += bytes_sent;

    } while (bytes_read > 0);

    return total_bytes_sent;
}

StreamContent* StreamContent_New(ClientSocket* source) {
    StreamContent* content = (StreamContent*)malloc(sizeof(StreamContent));

    if (content) {
        memset(content, 0, sizeof(StreamContent));

        BaseHttpContent(content)->_kind = _HTTP_CONTENT_KIND_FILE;

        content->_source = source;
    }

    return content;
}

size_t StreamContent_Read(const StreamContent* content, const byte* buffer, const size_t buffer_size) {
    RETURN_ZERO_IF_NULL(content);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);

    return ClientSocket_Read(content->_source, buffer, buffer_size);
}

char* StreamContent_ReadAsString(const StreamContent* content) {
    RETURN_NULL_IF_NULL(content);

    /*
        Read all the content into a string buffer.
    */
    size_t total_bytes_read = 0;
    size_t bytes_read = 0;
    size_t buffer_size = 0;
    size_t last_total = 0;
    char recv_buffer[1024] = { 0 };
    char* buffer = NULL;

    do {
        bytes_read = ClientSocket_Read(content->_source, recv_buffer, 1024);

        last_total = total_bytes_read;
        total_bytes_read += bytes_read;

        /*
            Reallocate the buffer if needed.
        */
        if (total_bytes_read > buffer_size) {
            buffer_size += bytes_read + 1;

            char* new_buffer = (buffer == 0) ?
                (char*)malloc(buffer_size) :
                (char*)realloc(buffer, buffer_size);

            BREAK_IF_NULL(new_buffer);

            buffer = new_buffer;

            buffer[buffer_size - 1] = '\0';
        }

        strncpy(buffer + last_total, recv_buffer, bytes_read);

    } while (bytes_read > 0);

    return buffer;
}

size_t StreamContent_ReadToFile(const StreamContent* content, const char* file_name) {
    RETURN_ZERO_IF_NULL(content);
    RETURN_ZERO_IF_NULL(file_name);

    /*
        Attempt to open the file in binary write mode.
    */
    FILE* file = fopen(file_name, "wb");
    size_t total_bytes_read = 0;

    if (file) {
        const byte buffer[CHUNK_SIZE_STREAM_TO_FILE_READ] = {0};
        size_t bytes_read = 0;

        do {
            bytes_read = ClientSocket_Read(content->_source, buffer, CHUNK_SIZE_STREAM_TO_FILE_READ);

            total_bytes_read += bytes_read;

        } while (bytes_read != 0);

        fclose(file);
    }

    return total_bytes_read;
}

void StreamContent_Delete(StreamContent* content) {
    RETURN_IF_NULL(content);

    ClientSocket_Close(content->_source);

    free(content);
}

size_t StreamContent_Send(StreamContent* content, ClientSocket* cs, u64 max_bytes_to_send) {
    RETURN_ZERO_IF_NULL(content);
    RETURN_ZERO_IF_NULL(cs);
    RETURN_ZERO_IF_ZERO(max_bytes_to_send);

    /*
        Send the N bytes (defined by content_length) from the source to the provided
        client socket.
    */
    size_t total_bytes_read = 0, bytes_read;
    size_t total_bytes_sent = 0, bytes_sent;

    byte* buffer[CHUNK_SIZE_STREAM_CONTENT_SEND] = { 0 };

    while(true) {
        
        bytes_read = ClientSocket_Read(content->_source, buffer, CHUNK_SIZE_STREAM_CONTENT_SEND);

        BREAK_IF_ZERO(bytes_read);

        total_bytes_read += bytes_read;

        if (max_bytes_to_send > 0) {
            /*
                Ensure that no more bytes are sent than the number of bytes specified in the
                "max_bytes_to_send".
            */
            size_t bytes_to_send = total_bytes_read < max_bytes_to_send ?
                bytes_read :
                total_bytes_read - max_bytes_to_send;

            bytes_sent = ClientSocket_Write(cs, buffer, bytes_to_send);

            total_bytes_sent += bytes_sent;

            if (total_bytes_read > max_bytes_to_send) {
                break;
            }
        }
        else {
            bytes_sent = ClientSocket_Write(cs, buffer, bytes_read);

            total_bytes_sent += bytes_sent;
        }

    }

    return total_bytes_sent;
}

size_t HttpContent_Send(HttpContent* content, ClientSocket* cs) {
    switch (content->_kind) {
        case _HTTP_CONTENT_KIND_BYTE_BUFFER:
            return ByteBufferContent_Send((ByteBufferContent*)content, cs);
        case _HTTP_CONTENT_KIND_FILE:
            return FileContent_Send((FileContent*)content, cs);
        case _HTTP_CONTENT_KIND_STREAM:
            return StreamContent_Send((StreamContent*)content, cs, STREAM_CONTENT_SEND_ALL);
    }

    return 0;
}

void HttpContent_Delete(HttpContent* content) {
    RETURN_IF_NULL(content);

    switch (content->_kind) {
        case _HTTP_CONTENT_KIND_BYTE_BUFFER:
            ByteBufferContent_Delete((ByteBufferContent*)content);
            break;
        case _HTTP_CONTENT_KIND_FILE:
            FileContent_Delete((FileContent*)content);
            break;
        case _HTTP_CONTENT_KIND_STREAM:
            StreamContent_Delete((StreamContent*)content);
            break;
    }
}
