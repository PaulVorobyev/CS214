#include <netinet/ip.h>
#ifndef __LIBNETFILES_H
#define __LIBNETFILES_H

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;
typedef unsigned char uchar;

typedef struct client {
    int sockfd;
    int portno;
    sockaddr_in* serv_addr;
    hostent* server;
    char* host;
} client;

int netopen(char* pathname, int mode);
ssize_t netwrite(int fd, void* buf, size_t nbyte);
ssize_t netread(int filedes, void* buf, size_t nbyte);
void netclose(int);
void netserverinit(char* host, int access_mode);
#endif
