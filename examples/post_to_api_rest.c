/*
	In this example we create a new product in the Platzi Fake Store API.

	For simplicity the product is created with a default image.
*/
#include <requester.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE_URL "https://api.escuelajs.co/api/v1"

size_t prompt(const char* message, char* buffer, size_t buffer_size) {
	size_t length = 0;

	do {
		fputs(message, stdout);

		fgets(buffer, buffer_size, stdin);

		buffer[strcspn(buffer, "\n")] = '\0';

		length = strlen(buffer);

	} while (length == 0);

	length = 0;
}

long int prompt_int(const char* message) {
	long int value = 0;
	char buffer[20] = { 0 };
	char* end_ptr = NULL;

	do {
		prompt(message, buffer, 20);

		value = strtol(buffer, &end_ptr, 10);

	} while (end_ptr == NULL);

	return value;
}

int main(void) {
	const char* BASE_JSON = "{ \"title\": \"%s\", \"price\": %d, \"description\": \"%s\", \"categoryId\": %d, \"images\": [ \"https://placeimg.com/640/480/any?r=0.9178516507833767\" ] }";

	char title[100] = { 0 };
	unsigned int price;
	char description[200] = { 0 };
	unsigned int category;

	prompt("Product title: ", title, 100);
	price = (unsigned int) prompt_int("Product price (int): ");
	prompt("Product description: ", description, 200);
	category = (unsigned int) prompt_int("Product category (int): ");

	puts("");

	/*
		Build the payload.
	*/
	size_t payload_len = strlen(BASE_JSON) + strlen(title) + strlen(description) + 40;

	char* payload = (char*)malloc(payload_len);

	memset(payload, 0, payload_len);

	sprintf(payload, BASE_JSON, title, price, description, category);

	/*
		Initialize library.
	*/
	HttpClient_Init();

	/*
		Send the request.
	*/
	const char* url = BASE_URL"/products";

	HttpResponse* res = HttpClient_Post(url, payload, MIME_APPLICATION_JSON);

	if (res == HTTP_INVALID_RESPONSE) {
		printf("Cannot get the response from %s\n", url);
		return -1;
	}

	/*
		Read the response.
	*/
	printf("%d %s\n", res->status, HttpStatus_ToString(res->status));

	/*
		Print the response content.
	*/
	char* content = StreamContent_ReadAsString(res->content);

	if (content != NULL) {
		printf("%s\n", content);
		free(content);
	}

	/*
		Release resources.
	*/
	free(payload);
	HttpResponse_Delete(res);
	HttpClient_Cleanup();
	
	return 0;
}