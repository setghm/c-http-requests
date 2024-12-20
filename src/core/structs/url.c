#include "url.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// requester
#include "url_protocol.h"
#include "query_params.h"
#include <http_default.h>
#include <utils/utils.h>

URL* URL_New(void) {
    URL* url = (URL*)malloc(sizeof(URL));

    if (url != NULL) {
        memset(url, 0, sizeof(URL));

        url->query_params = StringMap_New();
    }

    return url;
}

void URL_Delete(URL* url) {
    RETURN_IF_NULL(url);

    RELEASE_IF_NOT_NULL(url->fragment);
    RELEASE_IF_NOT_NULL(url->host);
    RELEASE_IF_NOT_NULL(url->password);
    RELEASE_IF_NOT_NULL(url->resource);
    RELEASE_IF_NOT_NULL(url->user);

    StringMap_Delete(url->query_params);

    free(url);
}

void URL_ResourceParse(URL* url, const char* src) {
    RETURN_IF_NULL(url);
    RETURN_IF_NULL(src);

    const size_t len = strlen(src);
    size_t res_len = 0, query_len;

    char* buffer = NULL;
    char* res_start = strstr(src, HTTP_DELIMITER_URL_RESOURCE);
    const char* query_start = strstr(src, HTTP_DELIMITER_URL_QUERY);
    const char* frag_start = strstr(src, HTTP_DELIMITER_URL_FRAGMENT);

    /*
        Get the resource path or set it (if empty).
    */
    if (res_start) {
        res_len = query_start ? query_start - res_start :
            frag_start ? frag_start - res_start :
            len - (size_t)(res_start - src);

        if (res_len != 0) {
            url->resource = strnclone(res_start, res_len);
        }
    }

    if (!res_start || res_len == 0) {
        url->resource = strclone("/");
    }

    /*
        Get the query parameters (if included) and parse.
    */
    if (query_start) {
        query_start++;

        query_len = frag_start ? frag_start - query_start : len - (size_t)(query_start - src);

        if (query_len != 0) {
            buffer = strnclone(query_start, query_len);

            StringMap* query_params = QueryParams_ParseNew(buffer, query_len);

            free(buffer);

            if (url->query_params != NULL) {
                StringMap_AddFrom(url->query_params, query_params);
                StringMap_Delete(query_params);
            } else {
                url->query_params = query_params;
            }
        }
    }

    /*
        Get the fragment (if included).
    */
    if (frag_start) {
        frag_start++;
        url->fragment = strclone(frag_start);
    }
}

URL* URL_ParseNew(const char* src) {
    RETURN_NULL_IF_NULL(src);

    URL* url = URL_New();
    char* buffer;

    const size_t src_len = strlen(src);

    /*
        Store all locations and lengths.
    */
    char *user_start = NULL,
         *pass_start = NULL,
         *host_start = NULL,
         *port_start = NULL,
         *res_start = NULL,
         *query_start = NULL,
         *frag_start = NULL;

    size_t proto_len = 0,
           user_len = 0,
           pass_len = 0,
           host_len = 0,
           port_len = 0;

    /*
        Get the protocol location (the URL schema).
    */
    char* delimiter = strstr(src, HTTP_DELIMITER_URL_PROTOCOL);

    if (delimiter) {
        proto_len = delimiter - src;
        host_start = delimiter + 3;

        buffer = strnclone(src, proto_len);

        url->protocol = URLProtocol_FromString(buffer, proto_len);

        free(buffer);
    }
    else {
        /*
            Malformed URL.
        */
        return url;
    }

    /*
        Get username and password (if included).
        First, check if the '@' character is present.
    */
    delimiter = strstr(host_start, HTTP_DELIMITER_URL_HOST);

    if (delimiter) {
        pass_start = strstr(host_start, HTTP_DELIMITER_URL_USERPWD);

        /*
            The password start must be before the '@' character.
        */
        if (pass_start && pass_start < delimiter) {
            /*
                Get the user.
            */
            user_start = host_start;
            user_len = pass_start - user_start;

            if (user_len != 0) {
                url->user = strnclone(user_start, user_len);
            }

            /*
                Get the password.
            */
            pass_start++;
            pass_len = delimiter - pass_start;

            if (pass_len != 0) {
                url->password = strnclone(pass_start, pass_len);
            }
        } else {
            /*
                Get the user only.
            */
            user_start = host_start;
            user_len = delimiter - user_start;

            if (user_len != 0) {
                url->user = strnclone(user_start, user_len);
            }
        }

        host_start = delimiter + 1;
    }

    /*
        Get the host.
    */
    port_start = strstr(host_start, HTTP_DELIMITER_URL_PORT);
    res_start = strstr(host_start, HTTP_DELIMITER_URL_RESOURCE);
    query_start = strstr(host_start, HTTP_DELIMITER_URL_QUERY);
    frag_start = strstr(host_start, HTTP_DELIMITER_URL_FRAGMENT);

    host_len = port_start ? port_start - host_start :
        res_start ? res_start - host_start :
        query_start ? query_start - host_start :
        frag_start  ? frag_start - host_start :
        src_len - (size_t)(host_start - src);

    if (host_len != 0) {
        url->host = strnclone(host_start, host_len);
    }

    /*
        Get the port (if included).
    */
    if (port_start) {
        port_start++;

        port_len = res_start ? res_start - port_start : query_start ? query_start - port_start
                                                    : frag_start    ? frag_start - port_start
                                                                    : src_len - (size_t)(port_start - src);

        if (port_len != 0) {
            buffer = strnclone(port_start, port_len);

            url->port = parse_u16(buffer);

            free(buffer);
        }
    }

    if (!port_start || port_len == 0) {
        url->port = (url->protocol == URL_PROTOCOL_HTTP) ? HTTP_PORT : HTTPS_PORT;
    }

    /*
        Parse the resource part.
    */
    if (res_start) {
        URL_ResourceParse(url, res_start);
    } else if (query_start) {
        URL_ResourceParse(url, query_start);
    } else if (frag_start) {
        URL_ResourceParse(url, frag_start);
    } else {
        url->resource = strclone("/");
    }

    return url;
}

char* URL_ToString(URL* url) {
    /*
        Calculate the length of the string.
    */
    size_t len = 1;  // NULL character.

    /*
        Get the string of the protocol.
    */
    const char* protocol_str = URLProtocol_ToString(url->protocol);

    len += strlen(protocol_str) + 3;  // Plus "://" separator

    /*
        Get the string for the resource path including the query parameters
        and the fragment.
    */
    char* res_str = URL_ResourceToString(url);

    if (res_str) {
        len += strlen(res_str);
    }

    /*
        Get the string of the host.
    */
    len += strlen(url->host);

    /*
        Get the string of the user (if included).
    */
    if (url->user != NULL) {
        len += strlen(url->user) + 1;  // Plus '@' character.

        /*
            Get the string of the password (if included).
        */
        if (url->password != NULL) {
            len += strlen(url->password) + 1;  // Plus ':' character.
        }
    }

    /*
        Include the port if url port and default protocol port doesn't match.
    */
    char* port_str = NULL;

    if ((url->protocol == URL_PROTOCOL_HTTP && url->port != HTTP_PORT) ||
        (url->protocol == URL_PROTOCOL_HTTPS && url->port != HTTPS_PORT) ||
        (url->protocol == URL_PROTOCOL_WS && url->port != HTTP_PORT) ||
        (url->protocol == URL_PROTOCOL_WS && url->port != HTTPS_PORT)) {
        /*
            Append the null character.
        */
        port_str = (char*)malloc(U16_MAX_CIPHERS + 1);

        if (port_str) {
            len += sprintf(port_str, "%hu", url->port);

            /*
                Add the ':' character.
            */
            len++;
        }
    }

    /*
        Alloc the string.
    */
    char* str = (char*)malloc(len);

    if (str) {
        size_t i = 0;

        memset(str, 0, len);

        /*
            Dump the values.
        */
        i += sprintf(str, "%s://", protocol_str);

        if (url->user) {
            if (url->password) {
                i += sprintf((str + i), "%s:%s@", url->user, url->password);
            } else {
                i += sprintf((str + i), "%s@", url->user);
            }
        }

        i += sprintf((str + i), "%s", url->host);

        if (port_str) {
            i += sprintf((str + i), ":%s", port_str);
        }

        if (res_str) {
            sprintf((str + i), "%s", res_str);
        }
    }

    /*
        Release strings.
    */
    RELEASE_IF_NOT_NULL(port_str);
    RELEASE_IF_NOT_NULL(res_str);

    return str;
}

char* URL_ResourceToString(const URL* url) {
    RETURN_NULL_IF_NULL(url);
    RETURN_NULL_IF_NULL(url->resource);

    /*
        Calculate the length of the string.
    */
    size_t len = 1;  // NULL character.

    len += strlen(url->resource);

    char* query_str = NULL;

    if (url->query_params) {
        query_str = QueryParams_ToString(url->query_params);

        /*
            Query parameters length plus '?' character.
        */
        len += strlen(query_str) + 1;
    }

    if (url->fragment) {
        /*
            Fragment length plus '#' character.
        */
        len += strlen(url->fragment) + 1;
    }

    /*
        Alloc the string.
    */
    char* str = (char*)malloc(len);

    if (str) {
        size_t i = 0;

        memset(str, 0, len);

        /*
            Dump the values.
        */
        i += sprintf(str, "%s", url->resource);

        if (url->query_params) {
            i += sprintf((str + i), "?%s", query_str);
        }

        if (url->fragment) {
            i += sprintf((str + i), "#%s", url->fragment);
        }
    }

    /*
        Release strings.
    */
    RELEASE_IF_NOT_NULL(query_str);

    return str;
}
