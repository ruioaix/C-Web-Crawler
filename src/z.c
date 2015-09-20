#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h> /* socket, connect */
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("usage:\n");
		exit(0);
	}

	struct addrinfo hints;
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

	int status;
	struct addrinfo *servinfo;  // will point to the results
	if ((status = getaddrinfo(argv[1], "80", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	char ipstr[INET6_ADDRSTRLEN];
	struct addrinfo *p;
	for(p = servinfo; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("  %s: %s\n", ipver, ipstr);
	}
	int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (sockfd == -1) {
		fprintf(stderr, "socket create error");
		exit(0);
	}
	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
		fprintf(stderr, "connect error");
		exit(0);
	}
	freeaddrinfo(servinfo); // free the linked-list

	char message[1024], response[114096];
	char *message_fmt = "HEAD / HTTP/1.0\r\n\r\n";
	sprintf(message, message_fmt, argv[1], argv[2]);
	printf("Request:\n%s\n", message);
	int total = strlen(message);
	int sent = 0, bytes;
	do {
		bytes = send(sockfd, message + sent, total-sent, 0);
		if (bytes < 0) {
			fprintf(stderr, "ERROR writing message to socket");
			exit(0);
		}
		if (bytes == 0) break;
		sent += bytes;
	} while (sent < total);

	memset(response, 0, sizeof(response));
	total = sizeof(response)-1;
	int received = 0;
	do {
		bytes = recv(sockfd, response+received, total-received, 0);
		if (bytes < 0) {
			fprintf(stderr, "ERROR reading response from socket");
			exit(0);
		}
		if (bytes == 0)
			break;
		received+=bytes;
	} while (received < total);

	if (received >= total) {
		fprintf(stderr, "ERROR storing complete response from socket");
		exit(0);
	}

	close(sockfd);

	printf("Response:\n%s\n",response);
	return 0;
}

