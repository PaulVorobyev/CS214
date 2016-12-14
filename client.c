#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "client.h"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;
typedef unsigned char uchar;

client* CLIENT = NULL;

client* create_client(char* host, int port) {
    client* cli = (client*)malloc(sizeof(client));
    cli->serv_addr = (sockaddr_in*)calloc(1, sizeof(sockaddr_in));
    cli->portno = port;
    cli->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cli->sockfd < 0) {
        printf("Unable to create socket\n");
        return NULL;
    }
    cli->server = gethostbyname(host);
    if (cli->server == NULL) {
        printf("No such host exists\n");
        return NULL;
    }
    cli->serv_addr->sin_family = AF_INET;
    cli->serv_addr->sin_port = htons(port);
    return cli;
}

char* get_response(client* cli) {
    short size = 0;
    read(cli->sockfd, &size, sizeof(short)); // getting size of payload
    printf("PAYLOAD SIZE %d\n", size);
    char* payload = (char*)malloc(size+2); // allocating space for payload
    memcpy(payload, &size, 2); // copying payload size
    read(cli->sockfd, payload+2, size); // reading in payload
    int i;
    for (i = 0; i < size+2; i++) {
        printf("%02x\n", payload[i]);
    }
    return payload;
}

void write_data(client* cli, char* data, int nbyte) {
    if (connect(cli->sockfd, (sockaddr*)cli->serv_addr, sizeof(*(cli->serv_addr))) == -1) {
        printf("Unable to connect.\n");
        return;
    }
    write(cli->sockfd, data, nbyte);
}

int netopen(char* pathname, int mode) {
    if (CLIENT != NULL) free(CLIENT);
    CLIENT = create_client("127.0.0.1", 2000);
    int pnsize = strlen(pathname);
    uchar header[8];
    header[4] = mode;
    header[5] = 1;
    header[6] = (char)((pnsize >> 8) & 0xff);
    header[7] = (char)(pnsize & 0xff);
    char* payload = (char*)malloc(pnsize+8);
    memcpy(payload, header, 8);
    memcpy(payload+8, pathname, pnsize);
    write_data(CLIENT, payload, pnsize+8);
    char* resp = get_response(CLIENT);
    ushort size = 0;
    memcpy(&size, resp, sizeof(size));
    int fd = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        printf("%d\n", resp[sizeof(size)+i]);
        fd = (fd << 8*i) | resp[sizeof(size)+i];
    }
    return fd;
}

ssize_t netwrite(int fildes, const void* buf, size_t nbyte) {
    if (CLIENT != NULL) free(CLIENT);
    CLIENT = create_client("127.0.0.1", 2000);
    printf("FILDES %d\n", fildes);
    uchar header[8];
    memcpy(header, &fildes, sizeof(fildes));
    header[4] = 2 << 4;
    header[5] = 1;
    header[6] = (char)((nbyte >> 8) & 0xff);
    header[7] = (char)(nbyte & 0xff);
    char payload[nbyte+8];
    memcpy(payload, header, 8);
    memcpy(payload+8, buf, nbyte);
    write_data(CLIENT, payload, nbyte+8);
    char* resp = get_response(CLIENT);
    int respv = 0;
    memcpy(&respv, resp, sizeof(respv));
    printf("RETURNING\n");
    return respv;
}

int main(int argc, char** argv) {
    printf("%d %d %d\n", O_RDONLY, O_WRONLY, O_RDWR);
    int fd = netopen("hello.txt", O_WRONLY);
    printf("FD %d\n", fd);
    int status = netwrite(fd, "Hello there", 11);
}
