#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include "randombytes.h"
#include "limits.h"

static unsigned char randombyte[1];

void limits(void) {

#ifdef RLIM_INFINITY
    struct rlimit r;
    r.rlim_cur = 0;
    r.rlim_max = 0;
#endif

    randombytes(randombyte, 1);

#ifdef RLIM_INFINITY

/* prohibit new files, new sockets, etc. */
#ifdef RLIMIT_NOFILE
    setrlimit(RLIMIT_NOFILE, &r);
#endif

 /* prohibit fork */
#ifdef RLIMIT_NPROC
    setrlimit(RLIMIT_NPROC, &r);
#endif

#endif
}
