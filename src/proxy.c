#include "proxy.h"
#include "safe.h"
#include "request.h"

#define STEP 2000

int crawl_proxy(proxy **proxylist, int *proxylen) {
	proxy *list = smalloc(STEP * sizeof(proxy));
	int len = STEP;

	char *domain = "pachong.org";
	//char *domain = "www.w3.org";
	int sfd = socketfd(domain);
	char *response;
	//char *path = "/Protocols/rfc2616/rfc2616.txt";
	char *path = "/";
	request(sfd, "GET", domain, path, &response);
	//FILE *fp = sfopen("xx", "w");
	//fprintf(fp, "%s",response);
	
	*proxylist = list;
	*proxylen = len;
	return 0;
}
