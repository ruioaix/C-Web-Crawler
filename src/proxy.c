#include "proxy.h"
#include "safe.h"
#include "request.h"

#define STEP 2000

int crawl_proxy(proxy **proxylist, int *proxylen) {
	proxy *list = smalloc(STEP * sizeof(proxy));
	int len = STEP;

	char *domain = "pachong.org";
	int sfd = socketfd(domain);
	char *response;
	char *path = "/";
	request(sfd, "GET", domain, path, &response);
	printf("Response:\n%s\n",response);
	
	*proxylist = list;
	*proxylen = len;
	return 0;
}
