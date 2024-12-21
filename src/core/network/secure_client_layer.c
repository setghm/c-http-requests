#include "secure_client_layer.h"
#include "certs_location.h"

void SecureClientLayers_Init() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void SecureClientLayers_Cleanup() {
    EVP_cleanup();
}

SecureClientLayer* SecureClientLayer_New(void) {
    SecureClientLayer* scs = (SecureClientLayer*)malloc(sizeof(SecureClientLayer));

    if (scs != NULL) {
        memset(scs, 0, sizeof(SecureClientLayer));
    }

    return scs;
}

void SecureClientLayer_Delete(SecureClientLayer* scl) {
    RETURN_IF_NULL(scl);

    SSL_shutdown(scl->_ssl);

    SSL_free(scl->_ssl);
    SSL_CTX_free(scl->_context);

    free(scl);
}

boolean SecureClientLayer_Connect(SecureClientLayer* scl, const int fd) {
    /*
        Create SSL context.
    */
    const SSL_METHOD* method;
    int result;

    method = SSLv23_client_method();

    scl->_context = SSL_CTX_new(method);

    if (scl->_context == NULL) {
        print_log(LOG_ERROR, "SecureClientLayer", "Unable to create the SSL context");
        ERR_print_errors_fp(stderr);
        return false;
    }

    /*
        Configure SSL context.
    */
    result = SSL_CTX_load_verify_locations(scl->_context, CERTS_LOCATION, NULL);

    if (!result) {
        print_log(LOG_ERROR, "SecureClientLayer", "Failed to load CA certificates");
        ERR_print_errors_fp(stderr);
        return false;
    }

    SSL_CTX_set_verify(scl->_context, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth(scl->_context, 1);

    /*
        Create a new SSL struct to handle this connection.
    */
    scl->_ssl = SSL_new(scl->_context);

    /*
        Set SSL file descriptor.
    */
    result = SSL_set_fd(scl->_ssl, fd);

    if (result == 0) {
        print_log(LOG_ERROR, "SecureClientLayer", "Cannot associate the file descriptor");
        ERR_print_errors_fp(stderr);
        return false;
    }

    if (SSL_connect(scl->_ssl) <= 0) {
        print_log(LOG_ERROR, "SecureClientLayer", "Cannot create a secure connection");
        ERR_print_errors_fp(stderr);
        return false;
    }

    return true;
}

size_t SecureClientLayer_Read(const SecureClientLayer* scl, byte* buffer, const size_t buffer_size) {
    RETURN_ZERO_IF_NULL(scl);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);

    const int bytes_read = SSL_read(scl->_ssl, buffer, (int)buffer_size);

    if (bytes_read < 0) {
        print_log(LOG_ERROR, "SecureClientLayer", "Error while reading incoming data");
        fprintf(stderr, "[ERROR][SecureClientLayer] Error code: %d\n", SSL_get_error(scl->_ssl, bytes_read));
        return 0;
    }

    return bytes_read;
}

size_t SecureClientLayer_Write(const SecureClientLayer* scl, const byte* buffer, const size_t buffer_size) {
    RETURN_ZERO_IF_NULL(scl);
    RETURN_ZERO_IF_NULL(buffer);
    RETURN_ZERO_IF_ZERO(buffer_size);

    const int bytes_written = SSL_write(scl->_ssl, buffer, (int)buffer_size);

    if (bytes_written < 0) {
        print_log(LOG_ERROR, "SecureClientLayer", "Error while reading incoming data");
        fprintf(stderr, "[ERROR][SecureClientLayer] Error code: %d\n", SSL_get_error(scl->_ssl, bytes_written));
        return 0;
    }

    return bytes_written;
}