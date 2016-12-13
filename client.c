#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

client* create_client(char* host, int port) {
    client* cli = (client*)malloc(sizeof(client));
    cli->hostname = (char*)malloc(strlen(host)+1);
    cli->hostname[strlen(host)] = '\0';
    memcpy(cli->hostname, host, strlen(host));
    cli->port = port;

    cli->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cli->sockfd < 0) {
        fprintf(stderr, "Error opening socket to file.");
        return NULL;
    }
    cli->s_addr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
    cli->s_addr.s_addr = inet_addr(host);
    cli->s_addr.SIN_FAMILY = AF_INET;
    cli->s_addr.SIN_PORT = htons(port);
    return cli;
}

void start_client(client* cli) {
    // connecting to server
    if (connect(cli->sockfd, (sockaddr*)cli->s_addr, sizeof(cli->s_addr)) == -1) {
        fprintf(stderr, "Unable to connect to socket.\n");
    }
}

char* write_to_server(client* cli, char* data) {

}

void close_client(client* cli) {

}

int open_file(client* cli, char* fp) {

}
