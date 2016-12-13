#include <netinet/ip.h>
#ifndef __CLIENT_H
#define __CLIENT_H

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct client {
    char* hostname;
    int port;
    sockaddr_in s_addr;
    int sockfd;
    int connfd;
} client;

client* create_client(char*,int);
void start_client(client*);
int open_file(client*,char*);
#endif
