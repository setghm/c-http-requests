#include <requester.h>
#include <stdio.h>

int main(void) {
	/*
		Initialize library.
	*/
	HttpClient_Init();

	/*
		Create a new http request and send it.
	*/
	//HttpResponse* res = HttpClient_Get("https://www.google.com/search?q=http+client");
	HttpResponse* res = HttpClient_Get("https://archlinux.org/packages/?q=aqua");

	if (res == HTTP_INVALID_RESPONSE) {
		puts("Cannot get the HttpResponse");
		return -1;
	}

	printf("Http response: %d\n", res->status);

	/*
		Print response headers.
	*/
	size_t i;

	printf("Http response headers: {\n");

	for (i = 0; i < res->headers->length; i++) {
		const StringPair* header = StringMap_GetAt(res->headers, i);

		printf("\t%s: \"%s\"\n", header->key, header->value);
	}

	puts("}");

	/*
		Get the response content.
	*/
	puts("------- Http response content -------");

	const char buffer[256] = { 0 };
	size_t bytes_read = 0;

	do {

		bytes_read = StreamContent_Read(res->content, buffer, 256);

		if (bytes_read > 0) {
			fputs(buffer, stdout);
		}

	} while (bytes_read == 256);

	puts("\n------- Http response content end -------\n");

	/*
		Cleanup resources.
	*/
	HttpResponse_Delete(res);
	HttpClient_Cleanup();

	return 0;
}
