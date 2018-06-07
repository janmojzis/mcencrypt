#include <stdlib.h>
#include "memreadall.h"
#include "die.h"
#include "writeall.h"
#include "fsyncfd.h"
#include "randombytes.h"

#define NAME "_randreplace"

static unsigned char *input = 0;
static unsigned long long inputlen = 0;

void cleanup(void) {

    if (input) {
        free(input);
        input = 0;
    }
}

int main(int argc, char **argv) {

    unsigned long a;
    unsigned long b;

    input = memreadall(&inputlen, 0, 0, 0);
    if (!input) die_temp(NAME, "read message failed");

    randombytes(&a, sizeof a);
    randombytes(&b, sizeof b);

    input[a % inputlen] += 1 + (b % 255);

    if (writeall(1, input, inputlen) == -1) die_temp(NAME, "write message failed");
    if (fsyncfd(1) == -1) die_temp(NAME, "write message failed");

    die(0);
}
