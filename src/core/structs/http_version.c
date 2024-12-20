#include "http_version.h"

#include <string.h>

// requester
#include "http_version_text.h"

HttpVersion HttpVersion_FromString(const char* src, const size_t src_size) {
	if (strncmp(src, HttpVersionText(HTTP_VERSION_1_0), src_size) == 0) {
		return HTTP_VERSION_1_0;
	}
	if (strncmp(src, HttpVersionText(HTTP_VERSION_1_1), src_size) == 0) {
		return HTTP_VERSION_1_1;
	}
	if (strncmp(src, HttpVersionText(HTTP_VERSION_2_0), src_size) == 0) {
		return HTTP_VERSION_2_0;
	}
	if (strncmp(src, HttpVersionText(HTTP_VERSION_3_0), src_size) == 0) {
		return HTTP_VERSION_3_0;
	}

	return HTTP_VERSION_UNKNOWN;
}

const char* HttpVersion_ToString(const HttpVersion version) {
	switch (version) {
		case HTTP_VERSION_0_9: return HTTP_VERSION_0_9_TEXT;
		case HTTP_VERSION_1_0: return HTTP_VERSION_1_0_TEXT;
		case HTTP_VERSION_1_1: return HTTP_VERSION_1_1_TEXT;
		case HTTP_VERSION_2_0: return HTTP_VERSION_2_0_TEXT;
		case HTTP_VERSION_3_0: return HTTP_VERSION_3_0_TEXT;
		default: return "";
	}
}