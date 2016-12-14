#include <netinet/ip.h>
#ifndef __CLIENT_H
#define __CLIENT_H

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;

typedef struct client {
    int sockfd;
    int portno;
    sockaddr_in* serv_addr;
    hostent* server;
} client;

#endif
