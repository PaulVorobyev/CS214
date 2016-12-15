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
    char* payload = (char*)malloc(size+2); // allocating space for payload
    memcpy(payload, &size, 2); // copying payload size
    read(cli->sockfd, payload+2, size); // reading in payload
    int i;
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
        fd = (fd << 8*i) | resp[sizeof(size)+i];
    }
    return fd;
}

ssize_t part_read(int fd, char* header, char* buf, ushort size, int offset) {
    if (CLIENT != NULL) free(CLIENT);
    CLIENT = create_client("127.0.0.1", 2000);
    write_data(CLIENT, (char*)header, 8);
    char* resp = get_response(CLIENT);
    resp += 2;
    if (resp[0] != 0) {
        fprintf(stderr, "Error encountered: code %d\n", resp[0]);
        return -1;
    }
    int rsize = ((unsigned int)resp[1] << 8) + resp[2];
    memcpy(buf+offset, resp+3, size);
    return rsize;
}

ssize_t netread(int filedes, void *buf, size_t nbyte) {
    uchar header[8];
    header[0] = (char)(filedes >> 24);
    header[1] = (char)(filedes >> 16);
    header[2] = (char)(filedes >> 8);
    header[3] = (char)filedes;
    header[4] = 1 << 4; 
    header[5] = 1;
    int offset = 0;
    int realsize = 0;
    while (nbyte > 0) { // breaks nbyte into multiple requests if the request size is above 4096
        // refreshing client
        if (CLIENT != NULL) {
            hostent* server = CLIENT->server;
            free(CLIENT);
            CLIENT = create_client("127.0.0.1", 2000);
            CLIENT->server = server;
        }

        int rdsize = (nbyte > 4096) ? 4096 : nbyte; // getting partial buffer size
        nbyte -= rdsize;
        header[6] = (char)((rdsize >> 8) & 0xff);
        header[7] = (char)(rdsize & 0xff);
        char* pthrdhdcpy = (char*)calloc(8, sizeof(char));
        memcpy(pthrdhdcpy, header, 8);
        int partsize = part_read(filedes, pthrdhdcpy, (char*)buf, rdsize, offset);
        if (partsize == -1) return -1;
        realsize += partsize;
        offset += rdsize;
    }    
    char* b = (char*)buf;
    int i;
    for (i = 0; i < realsize; i++) {
        putchar(b[i]);
    }
    return realsize;
}


int main(int argc, char** argv) {
    int foo2 = netopen("errant.txt", O_RDWR);
    void* buff = malloc(10000);
    netread(foo2, buff, 10000);
    
    int foo = netopen("sample.txt", O_RDWR);
    netread(foo, buff, 100);
}
