#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "client.h"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;

client* CLIENT = NULL;

client* create_client(char* host, int port) {
    client* cli = (client*)malloc(sizeof(client));
    cli->serv_addr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
    cli->portno = port;
    cli->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cli->sockfd < 0) {
        printf("NULL SOCKET\n");
        return NULL;
    }
    cli->server = gethostbyname(host);
    if (cli->server == NULL) {
        printf("NULL HOST\n");
        return NULL;
    }
    cli->serv_addr->sin_family = AF_INET;
    printf("B1\n");
    fflush(stdout);
    bcopy((char*)cli->server->h_addr, (char*)cli->serv_addr->sin_addr.s_addr, cli->server->h_length);
    printf("B2\n");
    fflush(stdout);
    cli->serv_addr->sin_port = htons(port);
    return cli;
}

void write_data(client* cli, char* data, int nbyte) {
    if (connect(cli->sockfd, (sockaddr*)cli->serv_addr, sizeof(*(cli->serv_addr))) == -1) {
        printf("Unable to connect.\n");
        return;
    }
    write(cli->sockfd, data, nbyte);
}


int main(int argc, char** argv) {
    CLIENT = create_client("localhost", 2000);
    return 0;
    write_data(CLIENT, "01234567", 8);
}
