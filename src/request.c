#include "request.h"
#include "log.h"
#include "safe.h"
#include <stdio.h>
#include <netdb.h>
#include <string.h>

int socketfd(char *domain) {
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

	int status;
	struct addrinfo *servinfo;  // will point to the results
	if ((status = getaddrinfo(domain, "80", &hints, &servinfo)) != 0) {
		LOG(LOG_FATAL, "getaddrinfo error: %s\n", gai_strerror(status));
	}

	//char ipstr[INET6_ADDRSTRLEN];
	//struct addrinfo *p;
	//for(p = servinfo; p != NULL; p = p->ai_next) {
	//	void *addr;
	//	char *ipver;

	//	// get the pointer to the address itself,
	//	// different fields in IPv4 and IPv6:
	//	if (p->ai_family == AF_INET) { // IPv4
	//		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
	//		addr = &(ipv4->sin_addr);
	//		ipver = "IPv4";
	//	} else { // IPv6
	//		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
	//		addr = &(ipv6->sin6_addr);
	//		ipver = "IPv6";
	//	}

	//	// convert the IP to a string and print it:
	//	inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
	//	printf("  %s: %s\n", ipver, ipstr);
	//}
	
	int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd == -1) {
		LOG(LOG_FATAL, "socket create error");
	}
	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
		LOG(LOG_FATAL, "connect error");
	}
	freeaddrinfo(servinfo); // free the linked-list
	return sockfd;
}

void request(int sfd, char *method, char *domain, char *path, char **response_retn) {

	char message[1024];
	//Request-Line = Method SP Request-URI SP HTTP-Version CRLF	
	char *header_common= "Cache-Control:max-age=0\r\n"
		"Connection:keep-alive\r\n"
		"Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
		"User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36\r\n";

	//request-line, header special, header common.
	sprintf(message, "%s %s HTTP/1.1\r\n%sHost:%s\r\n\r\n", method, path, header_common, domain);
	printf("%lu\n", strlen(message));
	printf("Request:\n%s\n", message);
	int total = strlen(message);
	int sent = 0, bytes;
	do {
		bytes = send(sfd, message + sent, total-sent, 0);
		if (bytes < 0) {
			LOG(LOG_FATAL, "ERROR send message to socket");
		}
		if (bytes == 0) break;
		sent += bytes;
	} while (sent < total);

	total = 100000;
	char *response = scalloc(total, sizeof(char));
	int received = 0;
	do {
		bytes = recv(sfd, response+received, total-received, 0);
		if (bytes < 0) {
			LOG(LOG_FATAL, "ERROR receive response from socket");
		}
		if (bytes == 0)
			break;
		received+=bytes;
	} while (received < total);

	if (received >= total) {
		LOG(LOG_FATAL, "ERROR storing complete response from socket");
	}

	*response_retn = response;
}
