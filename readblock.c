#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "readblock.h"


long long readblock(int fd, void *x, long long xlen) {

    long long r;
    char *buf = x;

    while (xlen > 0) {
        r = xlen;
        if (r > 1048576) r = 1048576; 
        r = read(fd, buf, r);
        if (r == 0) break;
        if (r == -1) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
                struct pollfd p;
                p.fd = fd;
                p.events = POLLIN;
                poll(&p, 1, -1);
                continue;
            }
            return -1;
        }
        buf += r; xlen -= r;
    }
    return (buf - (char *)x);
}

