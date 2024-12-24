#include <requester.h>
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