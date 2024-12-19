#include "http_status.h"
#include "http_status_text.h"

const char* HttpStatus_ToString(HttpStatus status) {
    switch (status) {
        /** INFORMATIONAL STATUSES 1XX **/
        case HTTP_STATUS_CONTINUE:
            return HttpStatusText(100);
        case HTTP_STATUS_SWITCHING_PROTOCOLS:
            return HttpStatusText(101);
        case HTTP_STATUS_PROCESSING:
            return HttpStatusText(102);
        case HTTP_STATUS_EARLY_HINTS:
            return HttpStatusText(103);

        /** SUCCESSFUL STATUSES 2XX **/
        case HTTP_STATUS_OK:
            return HttpStatusText(200);
        case HTTP_STATUS_CREATED:
            return HttpStatusText(201);
        case HTTP_STATUS_ACCEPTED:
            return HttpStatusText(202);
        case HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION:
            return HttpStatusText(203);
        case HTTP_STATUS_NO_CONTENT:
            return HttpStatusText(204);
        case HTTP_STATUS_RESET_CONTENT:
            return HttpStatusText(205);
        case HTTP_STATUS_PARTIAL_CONTENT:
            return HttpStatusText(206);
        case HTTP_STATUS_MULTI_STATUS:
            return HttpStatusText(207);
        case HTTP_STATUS_ALREADY_REPORTED:
            return HttpStatusText(208);
        case HTTP_STATUS_IM_USED:
            return HttpStatusText(226);

        /** REDIRECTION STATUSES 3XX **/
        case HTTP_STATUS_MULTIPLE_CHOICES:
            return HttpStatusText(300);
        case HTTP_STATUS_MOVED_PERMANENTLY:
            return HttpStatusText(301);
        case HTTP_STATUS_FOUND:
            return HttpStatusText(302);
        case HTTP_STATUS_SEE_OTHER:
            return HttpStatusText(303);
        case HTTP_STATUS_NOT_MODIFIED:
            return HttpStatusText(304);
        case HTTP_STATUS_USE_PROXY:
            return HttpStatusText(305);
        case HTTP_STATUS_TEMPORARY_REDIRECT:
            return HttpStatusText(307);
        case HTTP_STATUS_PERMANENT_REDIRECT:
            return HttpStatusText(308);

        /** CLIENT ERROR STATUSES 4XX **/
        case HTTP_STATUS_BAD_REQUEST:
            return HttpStatusText(400);
        case HTTP_STATUS_UNAUTHORIZED:
            return HttpStatusText(401);
        case HTTP_STATUS_PAYMENT_REQUIRED:
            return HttpStatusText(402);
        case HTTP_STATUS_FORBIDDEN:
            return HttpStatusText(403);
        case HTTP_STATUS_NOT_FOUND:
            return HttpStatusText(404);
        case HTTP_STATUS_METHOD_NOT_ALLOWED:
            return HttpStatusText(405);
        case HTTP_STATUS_NOT_ACCEPTABLE:
            return HttpStatusText(406);
        case HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED:
            return HttpStatusText(407);
        case HTTP_STATUS_REQUEST_TIMEOUT:
            return HttpStatusText(408);
        case HTTP_STATUS_CONFLICT:
            return HttpStatusText(409);
        case HTTP_STATUS_GONE:
            return HttpStatusText(410);
        case HTTP_STATUS_LENGTH_REQUIRED:
            return HttpStatusText(411);
        case HTTP_STATUS_PRECONDITION_FAILED:
            return HttpStatusText(412);
        case HTTP_STATUS_PAYLOAD_TOO_LARGE:
            return HttpStatusText(413);
        case HTTP_STATUS_URI_TOO_LONG:
            return HttpStatusText(414);
        case HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE:
            return HttpStatusText(415);
        case HTTP_STATUS_RANGE_NOT_SATISFIABLE:
            return HttpStatusText(416);
        case HTTP_STATUS_EXPECTATION_FAILED:
            return HttpStatusText(417);
        case HTTP_STATUS_IM_A_TEAPOT:
            return HttpStatusText(418);
        case HTTP_STATUS_MISDIRECTED_REQUEST:
            return HttpStatusText(421);
        case HTTP_STATUS_UNPROCESSABLE_ENTITY:
            return HttpStatusText(422);
        case HTTP_STATUS_LOCKED:
            return HttpStatusText(423);
        case HTTP_STATUS_FAILED_DEPENDENCY:
            return HttpStatusText(424);
        case HTTP_STATUS_TOO_EARLY:
            return HttpStatusText(425);
        case HTTP_STATUS_UPGRADE_REQUIRED:
            return HttpStatusText(426);
        case HTTP_STATUS_PRECONDITION_REQUIRED:
            return HttpStatusText(428);
        case HTTP_STATUS_TOO_MANY_REQUESTS:
            return HttpStatusText(429);
        case HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE:
            return HttpStatusText(431);
        case HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS:
            return HttpStatusText(451);

        /** SERVER ERROR STATUSES 5XX **/
        case HTTP_STATUS_INTERNAL_SERVER_ERROR:
            return HttpStatusText(500);
        case HTTP_STATUS_NOT_IMPLEMENTED:
            return HttpStatusText(501);
        case HTTP_STATUS_BAD_GATEWAY:
            return HttpStatusText(502);
        case HTTP_STATUS_SERVICE_UNAVAILABLE:
            return HttpStatusText(503);
        case HTTP_STATUS_GATEWAY_TIMEOUT:
            return HttpStatusText(504);
        case HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED:
            return HttpStatusText(505);
        case HTTP_STATUS_VARIANT_ALSO_NEGOTIATES:
            return HttpStatusText(506);
        case HTTP_STATUS_INSUFFICIENT_STORAGE:
            return HttpStatusText(507);
        case HTTP_STATUS_LOOP_DETECTED:
            return HttpStatusText(508);
        case HTTP_STATUS_NOT_EXTENDED:
            return HttpStatusText(510);
        case HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED:
            return HttpStatusText(511);
    }

    return HTTP_STATUS_UNKNOWN;
}
