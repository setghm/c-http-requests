#include "secure_client_layer.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

struct _SecureClientLayer {
    SSL_CTX* _context;

    SSL* _ssl;
};

void SecureClientLayers_Init() {
    SSL_library_init();
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

boolean SecureClientLayer_Connect(SecureClientLayer* scl, const int fd, const char* hostname, u16 port) {
    /*
        Create SSL context.
    */
    const SSL_METHOD* method;
    int result;

    method = TLS_client_method();

    scl->_context = SSL_CTX_new(method);

    if (scl->_context == NULL) {
        print_log(LOG_ERROR, "SecureClientLayer", "Unable to create the SSL context");
        ERR_print_errors_fp(stderr);
        return false;
    }

    /*
        Configure SSL context.
    */
    SSL_CTX_set_verify(scl->_context, SSL_VERIFY_NONE, NULL);
    
    //if (!SSL_CTX_set_default_verify_paths(scl->_context)) {
    //    print_log(LOG_ERROR, "SecureClientLayer", "Failed to set the default trusted certificate store");
    //    ERR_print_errors_fp(stderr);
    //    return false;
    //}

    /*
        Require TLS v1.2
    */
    if (!SSL_CTX_set_min_proto_version(scl->_context, TLS1_2_VERSION)) {
        print_log(LOG_ERROR, "SecureClientLayer", "Failed to set the minimum TLS version");
        ERR_print_errors_fp(stderr);
        return false;
    }

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

    /*
        Specify the target host.
    */
    if (!SSL_set_tlsext_host_name(scl->_ssl, hostname)) {
        print_log(LOG_ERROR, "SecureClientLayer", "Failed to set the SNI hostname");
        ERR_print_errors_fp(stderr);
        return false;
    }

    /*
        Ensure certificate for the target host.
    */
    if (!SSL_set1_host(scl->_ssl, hostname)) {
        print_log(LOG_ERROR, "SecureClientLayer", "Failed to set the certificate verification hostname");
        ERR_print_errors_fp(stderr);
        return false;
    }

    /*
        Do the handshake.
    */
    if (SSL_connect(scl->_ssl) <= 0) {
        print_log(LOG_ERROR, "SecureClientLayer", "Cannot create a secure connection");

        if (SSL_get_verify_result(scl->_ssl) != X509_V_OK) {
            fprintf(stderr, "Verify error: %s\n", X509_verify_cert_error_string(SSL_get_verify_result(scl->_ssl)));
        }

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
        ERR_print_errors_fp(stderr);
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