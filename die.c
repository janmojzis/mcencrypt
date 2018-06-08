#include <unistd.h>
#include "e.h"
#include "writeall.h"
#include "die.h"

static char buf[256];
static long long buflen = 0;

static void flush(void) {
    if (writeall(2, buf, buflen) == -1) _exit(111);
    buflen = 0;
}

static void outs(const char *x) {

    long long i;

    for(i = 0; x[i]; ++i) {
        if (buflen >= sizeof buf) flush();
        buf[buflen++] = x[i];
    }
}

extern void cleanup(void);

void die_usage(const char *name, const char *why, const char *usage) {
    cleanup();
    outs(name);
    outs(": usage: ");
    outs(name);
    outs(" ");
    outs(usage);
    outs("\n");
    outs(name);
    outs(": fatal: ");
    outs(why);
    outs(": ");
    outs(e_str(errno));
    outs("\n");
    flush();
    _exit(111);
}

void die_temp(const char *name, const char *why) {
    cleanup();
    outs(name);
    outs(": fatal: ");
    outs(why);
    if (errno) {
        outs(": ");
        outs(e_str(errno));
    }
    outs("\n");
    flush();
    _exit(111);
}

void die_perm(const char *name, const char *why) {
    cleanup();
    outs(name);
    outs(": fatal: ");
    outs(why);
    outs("\n");
    flush();
    _exit(100);
}

void die(int x) {
    cleanup();
    _exit(x);
}
