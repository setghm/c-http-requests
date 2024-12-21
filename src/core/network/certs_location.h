#ifndef _REQUESTER_NETWORK_CERTIFICATE_LOCATION_H
#define _REQUESTER_NETWORK_CERTIFICATE_LOCATION_H

#if defined(__linux__)
    #define CERTS_LOCATION "/etc/ssl/certs/ca-certificates.crt"
#elif defined(TARGET_OS_MAC)
    #define CERTS_LOCATION "/usr/local/etc/openssl/cert.pem"
#elif defined(_WIN32)
    #define CERTS_LOCATION ""
#else
    #define CERTS_LOCATION ""
#endif

#endif
