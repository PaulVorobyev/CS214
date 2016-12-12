#ifndef __LIBNETFILES_H
#define __LIBNETFILES_H

int netopen(const char* pathname, int flag);
ssize_t netread(int fd, void* buf, size_t nbyte);
ssize_t netwrite(int fd, const void* buf, size_t nbyte);
void netclose(int fd);

#endif
