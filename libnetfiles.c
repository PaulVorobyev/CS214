#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "libnetfiles.h"

int netopen(const char* pathname, int flag) {
    FILE* f = NULL;
    switch(flag) {
        case O_RDONLY:
            f = fopen(pathname, "r");
            break;
        case O_WRONLY:
            f = fopen(pathname, "w");
            break;
        case O_RDWR:
            f = fopen(pathname, "a+");
            break;
    }
    if (f == NULL) return -1;
    fseek(f, 0, SEEK_SET);
    return fileno(f);
}

ssize_t netread(int fd, void* buf, size_t nbyte) {
    if ((fcntl(fd, F_GETFL) & O_ACCMODE) == O_WRONLY) return -1; // check if readable
    return read(fd, buf, nbyte);
}

ssize_t netwrite(int fd, const void* buf, size_t nbyte) {
    if ((fcntl(fd, F_GETFL) & O_ACCMODE) == O_RDONLY) return -1; // check if writeable
    size_t pos = lseek(fd, 0, SEEK_CUR); // keeping track of read position
    lseek(fd, 0, SEEK_END); // seeks to the end to append
    size_t wrsize = write(fd, buf, nbyte);    
    lseek(fd, 0, pos); // seeks to read position
    return wrsize;
}

void netclose(int fd) {
    close(fd);
}

int main(int argc, char** argv) {
    int fd = netopen("sample.txt", O_WRONLY);
    printf("%d\n", fd);
    char* data = "Some new data.\n";
    netwrite(fd, data, strlen(data));
    netclose(fd);
    return 0;
}
