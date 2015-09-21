#ifndef __PROXY_H
#define __PROXY_H

#include <netinet/in.h>
typedef struct sockaddr_in proxy;

int crawl_proxy(proxy **proxylist, int *proxylen);

#endif
