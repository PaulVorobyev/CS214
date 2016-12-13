#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

client* CLIENT = NULL;

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

void set_client(client* cli) {
    CLIENT = cli;
}

void start_client(client* cli) {
    // connecting to server
    if (connect(cli->sockfd, (sockaddr*)cli->s_addr, sizeof(cli->s_addr)) == -1) {
        fprintf(stderr, "Unable to connect to socket.\n");
    }
}

void netopen(char* pathname, int flag) {
    char header[8];
    if (flag == O_RDONLY) flag = 0;
    else if (flag == O_RDONLY) flag = 1;
    else if (flag == O_WRONLY) flag = 2;
    else if (flag == O_RDWR) flag = 3;
    header[5] = flag;
    short pathlen = (short)strlen(pathname);
    header[6] = (char)((pathlen >> 8) & 0xff);
    header[7] = (char)(pathlen & 0xff);
    int payload_size = 8 + strlen(pathname); // getting payload size
    char* payload = (char*)malloc(payload_size); // mallocing payload
    // copying header info
    int i;
    for (i = 0; i < 8; i++) {
        payload[i] = header[i];
    }
    memcpy(payload+8, pathname, strlen(pathname)); // copying request body
    // sending data to server
    write(CLIENT->sockfd, payload, payload_size);
    char resp[4];
    read(CLIENT->sockfd, resp, 4); // getting file descriptor
    int fd = 0;
    for (i = 0; i < 4; i++) {
        fd = (fd << 8) | resp[i];
    }
    return fd;
}

ssize_t netwrite(int fd, void* buf, size_t nbyte) {

}

ssize_t netread(int fd, void* buf, size_t nbyte) {

}

int netclose(int fd) {

}
