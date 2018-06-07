#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "writeall.h"
#include "checkfd.h"
#include "memreadall.h"
#include "readblock.h"
#include "e.h"
#include "die.h"
#include "max.h"
#include "fsyncfd.h"
#include "randombytes.h"
#include "chacha20.h"
#include "poly1305.h"
#include "crypto_kem_mceliece8192128sha512.h"

#define NAME "mcdecrypt"

#if chacha20_KEYBYTES + chacha20_NONCEBYTES > crypto_kem_mceliece8192128sha512_BYTES
#error chacha20_KEYBYTES + chacha20_NONCEBYTES > crypto_kem_mceliece8192128sha512_BYTES
#endif

#if poly1305_KEYBYTES > chacha20_BLOCKBYTES
#error poly1305_KEYBYTES > chacha20_BLOCKBYTES
#endif

static unsigned char *map = MAP_FAILED;
static unsigned long long maplen = 0;

static unsigned char *buf = 0;

static unsigned char *input = 0;
static unsigned long long inputlen = 0;

static struct global {
    unsigned char sk[crypto_kem_mceliece8192128sha512_SECRETKEYBYTES];
    unsigned char k[crypto_kem_mceliece8192128sha512_BYTES];
    unsigned char ak[chacha20_BLOCKBYTES];

    unsigned char output[16384 * chacha20_BLOCKBYTES];

    chacha20_ctx chacha20ctx;
} g;


void cleanup(void) {
    randombytes(&g, sizeof g);

    if (map != MAP_FAILED) {
        munmap(map, maplen);
        map = MAP_FAILED;
        maplen = 0;
    }

    if (buf) {
        free(buf);
        buf = 0;
    }
}

int main(int argc, char **argv) {

    struct stat st;

    if (!checkfd(8, POLLIN)) die_usage(NAME, "read secretkey failed", "<ciphertext 8<secretkey >message");

    /* mmap input */
    if (fstat(0, &st) == 0) {
        maplen = st.st_size;
        map = mmap(0, maplen, PROT_READ, MAP_SHARED, 0, 0);
        if (map != MAP_FAILED) {
            input = map;
            inputlen = maplen;
        }
    }
    /* fallback to malloc(3) */
    if (!input) {
        buf = memreadall(&inputlen, 0, 0, 0);
        if (!buf) die_temp(NAME, "read ciphertext failed");
        input = buf;
    }

    if (inputlen > MAX) { errno = EPROTO; die_perm(NAME, "input message too long"); }
    if (inputlen < crypto_kem_mceliece8192128sha512_CIPHERTEXTBYTES + poly1305_BYTES) { errno = EPROTO; die_perm(NAME, "short ciphert text"); }
    inputlen -= crypto_kem_mceliece8192128sha512_CIPHERTEXTBYTES + poly1305_BYTES;

    /* get secret key */
    if (readblock(8, g.sk, sizeof g.sk) != sizeof g.sk) die_temp(NAME, "read secretkey failed");

    /* KEM */
    if (crypto_kem_mceliece8192128sha512_dec(g.k, input, g.sk) != 0) {
        randombytes(g.k, sizeof g.k);
    }
    randombytes(g.sk, sizeof g.sk);
    input += crypto_kem_mceliece8192128sha512_CIPHERTEXTBYTES;

    /* symetric key and nonce for encryption */
    chacha20_init(&g.chacha20ctx, /* key */g.k, /* nonce */g.k + chacha20_KEYBYTES);
    randombytes(g.k, sizeof g.k);

    /* symetric key for authentication */
    memset(g.ak, 0, sizeof g.ak);
    chacha20_blocks(&g.chacha20ctx, g.ak, g.ak, sizeof g.ak);

    /* verify authenticator */
    if (poly1305_verify(input + inputlen, input, inputlen, g.ak) != 0) die_perm(NAME, "decryption failed");
    randombytes(g.ak, sizeof g.ak);

    /* read encrypted input, decrypt and write output */
    while (inputlen >= sizeof g.output) {
        chacha20_blocks(&g.chacha20ctx, g.output, input, sizeof g.output);
        if (writeall(1, g.output, sizeof g.output) == -1) die_temp(NAME, "write message failed");
        inputlen -= sizeof g.output;
        input += sizeof g.output;
    }
    if (inputlen > 0) {
        chacha20_blocks(&g.chacha20ctx, g.output, input, inputlen);
        if (writeall(1, g.output, inputlen) == -1) die_temp(NAME, "write message failed");
    }
    if (fsyncfd(1) == -1) die_temp(NAME, "write message failed");
    die(0);
}
