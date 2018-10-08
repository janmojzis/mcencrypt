#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "max.h"
#include "memreadall.h"

#define CHUNK 4096

unsigned char *memreadall(unsigned long long *len, int fd) {

    unsigned char *buf = 0;
    unsigned char *newbuf;
    unsigned long long bufalloc = 0;
    unsigned long long pos = 0;
    long long r;

    for (;;) {
        if (bufalloc <= pos + CHUNK) {
            while (bufalloc <= pos + CHUNK) {
                bufalloc = bufalloc * 2 + CHUNK;
                if (bufalloc != (unsigned long long)(size_t)bufalloc) goto nomem;
                if (bufalloc >= MAX) goto nomem;
            }
            newbuf = realloc(buf, bufalloc);
            if (!newbuf) goto nomem;
            buf = newbuf;
        }

        r = read(fd, buf + pos, CHUNK);
        if (r == -1) { free(buf); return 0; }
        if (r == 0) { *len = pos; return buf; }
        pos += r;
    }
    

nomem:
    errno = ENOMEM;
    return 0;
}

