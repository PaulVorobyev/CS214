#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include "server.h"

server* init_server(char* hostname, int port, int max_connections) {
    // creating server object
    server* srv = (server*)malloc(sizeof(server));
    srv->hostname = (char*)malloc(strlen(hostname)+1);
    memcpy(srv->hostname, hostname, strlen(hostname)+1);
    srv->port = port;
    srv->s_addr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
    srv->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv->sockfd == -1) {
        fprintf(stderr, "Unable to create server.\n");
        return NULL;
    }

    srv->s_addr->sin_family = AF_INET;
    srv->s_addr->sin_addr.s_addr = htonl(INADDR_ANY); // replace with hostname
    srv->s_addr->sin_port = htons(port);
    int reuse = 1;
    if (setsockopt(srv->sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        fprintf(stderr, "Failed to set up socket options, proceeding with creation.\n");
    }

    int status = bind(srv->sockfd, (sockaddr*)srv->s_addr, sizeof(*srv->s_addr));
    if (status != 0) {
        fprintf(stderr, "Unable to bind.\n");
        return NULL;
    }

    status = listen(srv->sockfd, max_connections);
    if (status != 0) {
        fprintf(stderr, "Failed to to listen to socket.\n");
        return NULL;
    }
    srv->fdsize = 100;
    srv->fds = (int*)malloc(sizeof(int)*srv->fdsize);
    memset(srv->fds, -100, srv->fdsize);
    return srv;
}

void append_to_fdlist(server* srv, int fd) {
    int i;
    for (i = 0; i < srv->fdsize; i++) {
        if (srv->fds[i] == -100) {
            break;
        }
    }
    if (i == srv->fdsize) { // more room available, reallocate
        srv->fds = (int*)realloc(srv->fds, srv->fdsize+100);
        srv->fds[srv->fdsize] = fd;
        srv->fdsize += 100;
    } else {
        srv->fds[i] = fd;
    }
}

int check_fd_validity(server* srv, int fd) {
    int i;
    for (i = 0; i < srv->fdsize; i++) {
        if (srv->fds[i] == fd) {
            return 1;
        }
    }
    return 0;
}

void remove_from_fdlist(server* srv, int fd) {
    int i;
    for (i = 0; i < srv->fdsize; i++) {
        if (srv->fds[i] == fd) {
            srv->fds[i] = -100;
        }
    }
}

void handle_connection(server* srv, int connfd) {
    // 4 bytes: file descriptor
    // 1/2 byte: empty
    // 1/2 byte: file mode
    // 1 byte: max allowed connections
    // 2 bytes: size of data 
    int header_size = 8;
    char header[header_size];
    ssize_t rd = read(connfd, header, header_size);
    if (rd != 8) {
        // return ERROR status
        char* error = "Invalid header length. Must be of length 8.";
        write(connfd, error, strlen(error));
        close(connfd);
        return;
    }
    int i;
    unsigned fd = 0;
    for (i = 0; i < 4; i++) {
        fd = (fd << 8) | header[i];
    }
    char mode = header[4] & 0x0f; int connections = header[5];
    unsigned size = (header[6] << 8) + header[7];
    if (size > 4096) {
        // error, too large packet size
    }
    if (mode == 0) { // open file
        char fp[size+1];
        read(connfd, fp, size); // get filename
        fp[size] = '\0';
        FILE* file = NULL;
        if (mode == O_WRONLY) file = fopen(fp, "w");
        else if (mode == O_RDONLY) file = fopen(fp, "r");
        else if (mode == O_RDWR) file = fopen(fp, "a+");
        int fildes = -1;
        if (file != NULL) fildes = fileno(file);
        append_to_fdlist(srv, fildes);
        write(connfd, (void*)&fildes, sizeof(fildes));
    } else if (mode == 1) { // read
        if (!check_fd_validity(srv, fd)) {
            int error = -1;
            write(connfd, error, sizeof(error));
        } else {
            char buf[size+1];
            buf[size] = '\0';
            read(fd, buf, strlen(buf));
            write(connfd, buf, strlen(buf));
        }
    } else if (mode == 2) { // write
        if (!check_fd_validity(srv, fd)) {
            int error = -1;
            write(connfd, error, sizeof(error));
        } else {
            char buf[size+1];
            buf[size] = '\0';
            read(connfd, buf, strlen(buf)); // reads the data to write to file
            write(fd, buf, strlen(buf)); // writes it to file
        }
    } else if (mode == 3) { // close
        if (!check_fd_validity(srv, fd)) {
            int error = -1;
            write(connfd, error, sizeof(error));
        } else {
            close(fd);
        }
    } else {
        int error = -2;
        write(connfd, error, sizeof(error));
    }
    close(connfd);
}

void start_server(server* srv) {
    printf("Starting server on PORT %d with PID %d\n", srv->port, getpid());
    int connfd;
    int header_size = 8;
    for(;;) {
        connfd = accept(srv->sockfd, (sockaddr*)NULL, NULL);
        handle_connection(srv, connfd);
    }
}

int main(int argc, char** argv) {
    server* srv = init_server("localhost", 2000, 200);
    start_server(srv);
    return 0;
}
