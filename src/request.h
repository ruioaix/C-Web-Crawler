#ifndef __REQUEST_H
#define __REQUEST_H 

int socketfd(char *domain);
void request(int sfd, char *method, char *domain, char *path, char **response_retn);

#endif
