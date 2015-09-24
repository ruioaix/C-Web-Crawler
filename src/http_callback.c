#include "http_callback.h"
#include "log.h"
#include <stdio.h>

int on_message_begin(http_parser* parser) {
	if (parser->type == HTTP_RESPONSE) {
		LOG(LOG_INFO, "http response begin to prase");
	}
	return 0;
}

int on_header_field(http_parser *parser, const char *at, size_t length) {
	char hf[100];
	snprintf(hf, length + 1, "%s", at);
	LOG(LOG_INFO, "%s", hf);
	return 0;	
}

int on_header_value(http_parser *parser, const char *at, size_t length) {
	char hf[1000];
	snprintf(hf, length + 1, "%s", at);
	LOG(LOG_INFO, "%s", hf);
	LOG(LOG_INFO, "%d", parser->flags);
	return 0;	
}
