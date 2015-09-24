#ifndef HTTP_CALLBACK_H
#define HTTP_CALLBACK_H 

#include "http_parser.h"

int on_message_begin(http_parser*);
int on_header_field(http_parser*, const char *at, size_t length);
int on_header_value(http_parser*, const char *at, size_t length);

#endif
