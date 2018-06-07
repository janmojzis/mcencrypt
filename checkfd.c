#include <unistd.h>
#include <errno.h>
#include "checkfd.h"

int checkfd(int fd, short event) {

    struct pollfd p;
    char ch = 0;
    int r;

    p.fd = fd;
    p.events = event;
    p.revents = 0;
    for (;;) {
        r = poll(&p, 1, 0);
        if (r == -1) {
            if (errno == EAGAIN) continue;
            if (errno == EINTR) continue;
        }
        if (r != 1) return 0;
        if (r == 1) break;
    }
    if (p.revents != event) {
        if (event == POLLOUT) write(fd, &ch, 1);
        if (event == POLLIN) read(fd, &ch, 1);
        return 0;
    }
    return 1;
}
