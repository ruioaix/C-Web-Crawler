#include "log.h"
enum LOGLEVEL loglevel = LOG_INFO;
FILE *logfile;

#include "proxy.h"
int main(int argc, char *argv[]) {
	logfile = stdout;
	proxy *proxylist;
	int proxylen;
	return crawl_proxy(&proxylist, &proxylen);
}
