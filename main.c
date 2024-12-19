#include <requester.h>
#include <stdio.h>

int main(void) {
	HttpRequest* req = HttpRequest_New();

	req->url = URL_ParseNew("https://archlinux.org/packages/?q=aqua");

	HttpResponse* res = HttpClient_Send(req);

	if (res == HTTP_INVALID_RESPONSE) {
		puts("Cannot get the HttpResponse");
		return 0;
	}

	printf("Http response: %d\n", res->status);

	/*
		Print response headers.
	*/
	size_t i;

	printf("Http response headers: {\n");

	for (i = 0; i < res->headers->length; i++) {
		StringPair* header = StringMap_GetAt(res->headers, i);

		printf("\t%s: \"%s\"\n", header->key, header->value);
	}

	puts("}");

	/*
		Get the response content.
	*/
	puts("------- Http response content -------");

	char* buffer[256] = { 0 };
	size_t bytes_read = 0;

	do {

		bytes_read = StreamContent_Read(res->content, buffer, 256);

		if (bytes_read > 0) {
			fputs(buffer, stdout);
		}

	} while (bytes_read != 0);

	puts("\n------- Http response content end -------\n");

	HttpRequest_Delete(req);
	HttpResponse_Delete(res);

	return 0;
}
