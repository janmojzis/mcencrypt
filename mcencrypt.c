#include <string.h>
#include "writeall.h"
#include "checkfd.h"
#include "readblock.h"
#include "fsyncfd.h"
#include "die.h"
#include "max.h"
#include "e.h"
#include "randombytes.h"
#include "chacha20.h"
#include "poly1305.h"
#include "crypto_kem_mceliece8192128sha512.h"

#define NAME "mcencrypt"

#if chacha20_KEYBYTES + chacha20_NONCEBYTES > crypto_kem_mceliece8192128sha512_BYTES
#error chacha20_KEYBYTES + chacha20_NONCEBYTES > crypto_kem_mceliece8192128sha512_BYTES
#endif

#if poly1305_KEYBYTES > chacha20_BLOCKBYTES
#error poly1305_KEYBYTES > chacha20_BLOCKBYTES
#endif

static unsigned long long inputlen = 0;

static struct global {
    unsigned char pk[crypto_kem_mceliece8192128sha512_PUBLICKEYBYTES];
    unsigned char k[crypto_kem_mceliece8192128sha512_BYTES];
    unsigned char ak[chacha20_BLOCKBYTES];
    unsigned char c[crypto_kem_mceliece8192128sha512_CIPHERTEXTBYTES];
    unsigned char a[poly1305_BYTES];
    unsigned char buf[16384 * chacha20_BLOCKBYTES];

    chacha20_ctx chacha20ctx;
    poly1305_ctx poly1305ctx;
} g;

void cleanup(void) {
    randombytes(&g, sizeof g);
}

int main(int argc, char **argv) {

    long long r;

    /* get publickey */
    if (!checkfd(4, POLLIN)) die_usage(NAME, "read publickey failed", "<message 4<publickey >ciphertext");
    if (readblock(4, g.pk, sizeof g.pk) != sizeof g.pk) die_temp(NAME, "read publickey failed");

    /* KEM - generate ciphertext + symetric key */
    crypto_kem_mceliece8192128sha512_enc(g.c, g.k, g.pk);
    if (writeall(1, g.c, sizeof g.c) == -1) die_temp(NAME, "write ciphertext failed");
    randombytes(g.c, sizeof g.c);

    /* symetric key and nonce for encryption */
    chacha20_init(&g.chacha20ctx, g.k, g.k + chacha20_KEYBYTES);
    randombytes(g.k, sizeof g.k);

    /* symetric key for authentication */
    memset(g.ak, 0, sizeof g.ak);
    chacha20_blocks(&g.chacha20ctx, g.ak, g.ak, sizeof g.ak); /* XXX warning - 'sizeof g.ak' must be power of chacha20_BLOCKBYTES */
    poly1305_init(&g.poly1305ctx, g.ak);
    randombytes(g.ak, sizeof g.ak);

    /* read input and write encrypted output */
    for (;;) {
        r = readblock(0, g.buf, sizeof g.buf);
        if (r == -1) die_temp(NAME, "read message failed");
        inputlen += r;
        if (inputlen > MAX) { errno = EPROTO; die_perm(NAME, "read message failed"); }
        chacha20_blocks(&g.chacha20ctx, g.buf, g.buf, r);
        poly1305_blocks(&g.poly1305ctx, g.buf, r);
        if (writeall(1, g.buf, r) == -1) die_temp(NAME, "write ciphertext failed");
        if (r != sizeof g.buf) break;
    }

    /* write authenticator */
    poly1305_final(&g.poly1305ctx, g.a);
    if (writeall(1, g.a, sizeof g.a) == -1) die_temp(NAME, "write ciphertext failed");
    if (fsyncfd(1) == -1) die_temp(NAME, "write ciphertext failed");

    die(0);
}
