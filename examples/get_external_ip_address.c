#include <requester/requester.h>

int main(void) {
    HttpResponse* res = Http_Get("https://ifconfig.me/ip");

    if (res == HTTP_INVALID_RESPONSE) {
        return -1;
    }

    const char buffer[128] = { 0 };

    StreamContent_Read(res->content, buffer, 128);

    puts(buffer);

    return 0;
}