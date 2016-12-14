#include <netinet/ip.h>
#ifndef __SERVER_H
#define __SERVER_H

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

typedef struct server {
    char* hostname;
    int port;
    sockaddr_in* s_addr;
    int sockfd;
    int* fds;
    int fdsize;
} server;

server* init_server(char*,int,int);
void start_server(server*);
void stop_server(server*);
void teardown(server*); // cleanup

#endif
