#include "writeall.h"
#include "checkfd.h"
#include "fsyncfd.h"
#include "die.h"
#include "randombytes.h"
#include "crypto_kem_mceliece8192128sha512.h"

#define NAME "mckeypair"

static unsigned char pk[crypto_kem_mceliece8192128sha512_PUBLICKEYBYTES];
static unsigned char sk[crypto_kem_mceliece8192128sha512_SECRETKEYBYTES];

void cleanup(void) {
    randombytes(pk, sizeof pk);
    randombytes(sk, sizeof sk);
}

int main(int argc, char **argv) {

    if (!checkfd(5, POLLOUT)) die_usage(NAME, "write publickey failed", "5>publickey 9>secretkey");
    if (!checkfd(9, POLLOUT)) die_usage(NAME, "write secretkey failed", "5>publickey 9>secretkey");
    if (crypto_kem_mceliece8192128sha512_keypair(pk, sk)) die_temp(NAME, "keypair failed");
    if (writeall(5, pk, sizeof pk) == -1) die_temp(NAME, "write publickey failed");
    if (fsyncfd(5) == -1) die_temp(NAME, "write publickey failed");
    if (writeall(9, sk, sizeof sk) == -1) die_temp(NAME, "write secretkey failed");
    if (fsyncfd(9) == -1) die_temp(NAME, "write secretkey failed");
    die(0);
}
