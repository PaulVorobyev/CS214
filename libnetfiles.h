#ifndef __LIBNETFILES_H
#define __LIBNETFILES_H

typedef enum {
    O_RDONLY,
    O_WRONLY,
    O_RDWR
} FLAG;

int netopen(const char* pathname, FLAG flag);
ssize_t netread(int fd, void* buf, size_t nbyte);
ssize_t netwrite(int fd, const void* buf, size_t nbyte);
void netclose(int fd);

#endif
