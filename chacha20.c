/*
20180104
Jan Mojzis
Public domain.
*/

#include "chacha20.h"

static unsigned long unpack32(const unsigned char *x) {
    return
        (unsigned long) (x[0])                  \
    | (((unsigned long) (x[1])) << 8)           \
    | (((unsigned long) (x[2])) << 16)          \
    | (((unsigned long) (x[3])) << 24);
}
static void pack32(unsigned char *x, unsigned long u) {
    x[0] = u; u >>= 8;
    x[1] = u; u >>= 8;
    x[2] = u; u >>= 8;
    x[3] = u;
}

#define ROTATE32(x, c) ((x) << (c)) | (((x) & 0xffffffff) >> (32 - (c)))

#define QUARTERROUND(a, b, c, d)                    \
    a += b; d = ROTATE32(d ^ a, 16);                \
    c += d; b = ROTATE32(b ^ c, 12);                \
    a += b; d = ROTATE32(d ^ a,  8);                \
    c += d; b = ROTATE32(b ^ c,  7);

#define TWOROUNDS                                   \
    QUARTERROUND( x0, x4,  x8, x12)                 \
    QUARTERROUND( x1, x5,  x9, x13)                 \
    QUARTERROUND( x2, x6, x10, x14)                 \
    QUARTERROUND( x3, x7, x11, x15)                 \
    QUARTERROUND( x0, x5, x10, x15)                 \
    QUARTERROUND( x1, x6, x11, x12)                 \
    QUARTERROUND( x2, x7,  x8, x13)                 \
    QUARTERROUND( x3, x4,  x9, x14)

#define XORBLOCK(o, i)                              \
    x0  = s0;                                       \
    x1  = s1;                                       \
    x2  = s2;                                       \
    x3  = s3;                                       \
    x4  = k0;                                       \
    x5  = k1;                                       \
    x6  = k2;                                       \
    x7  = k3;                                       \
    x8  = k4;                                       \
    x9  = k5;                                       \
    x10 = k6;                                       \
    x11 = k7;                                       \
    x12 = n0;                                       \
    x13 = n1;                                       \
    x14 = n2;                                       \
    x15 = n3;                                       \
                                                    \
    TWOROUNDS /* round  1,  2 */                    \
    TWOROUNDS /* round  3,  4 */                    \
    TWOROUNDS /* round  5,  6 */                    \
    TWOROUNDS /* round  7,  8 */                    \
    TWOROUNDS /* round  9, 10 */                    \
    TWOROUNDS /* round 11, 12 */                    \
    TWOROUNDS /* round 13, 14 */                    \
    TWOROUNDS /* round 15, 16 */                    \
    TWOROUNDS /* round 17, 18 */                    \
    TWOROUNDS /* round 19, 20 */                    \
                                                    \
    pack32(o     ,  (x0 + s0) ^ unpack32(i     ));  \
    pack32(o +  4,  (x1 + s1) ^ unpack32(i +  4));  \
    pack32(o +  8,  (x2 + s2) ^ unpack32(i +  8));  \
    pack32(o + 12,  (x3 + s3) ^ unpack32(i + 12));  \
    pack32(o + 16,  (x4 + k0) ^ unpack32(i + 16));  \
    pack32(o + 20,  (x5 + k1) ^ unpack32(i + 20));  \
    pack32(o + 24,  (x6 + k2) ^ unpack32(i + 24));  \
    pack32(o + 28,  (x7 + k3) ^ unpack32(i + 28));  \
    pack32(o + 32,  (x8 + k4) ^ unpack32(i + 32));  \
    pack32(o + 36,  (x9 + k5) ^ unpack32(i + 36));  \
    pack32(o + 40, (x10 + k6) ^ unpack32(i + 40));  \
    pack32(o + 44, (x11 + k7) ^ unpack32(i + 44));  \
    pack32(o + 48, (x12 + n0) ^ unpack32(i + 48));  \
    pack32(o + 52, (x13 + n1) ^ unpack32(i + 52));  \
    pack32(o + 56, (x14 + n2) ^ unpack32(i + 56));  \
    pack32(o + 60, (x15 + n3) ^ unpack32(i + 60));

#define _CHACHA_BLOCK(inttype, fcename)             \
static void fcename( chacha20_ctx *ctx,             \
                    unsigned char *c,               \
                    const unsigned char *m,         \
                    unsigned long long l) {         \
                                                    \
    inttype x0, x1, x2, x3, x4, x5, x6, x7;         \
    inttype x8, x9, x10, x11, x12, x13, x14, x15;   \
    inttype k0 = ctx->k[0];                         \
    inttype k1 = ctx->k[1];                         \
    inttype k2 = ctx->k[2];                         \
    inttype k3 = ctx->k[3];                         \
    inttype k4 = ctx->k[4];                         \
    inttype k5 = ctx->k[5];                         \
    inttype k6 = ctx->k[6];                         \
    inttype k7 = ctx->k[7];                         \
    inttype n2 = ctx->n[0];                         \
    inttype n3 = ctx->n[1];                         \
    inttype s0 = 0x61707865;                        \
    inttype s1 = 0x3320646E;                        \
    inttype s2 = 0x79622D32;                        \
    inttype s3 = 0x6B206574;                        \
    unsigned long long u = ctx->u;                  \
    inttype n0 = (inttype)(u & 0xffffffff);         \
    inttype n1 = (inttype)(u >> 32);                \
                                                    \
    if (!l) return;                                 \
                                                    \
    while (l >= 64) {                               \
        XORBLOCK(c, m);                             \
                                                    \
        ++u;                                        \
        n0 = (inttype)(u & 0xffffffff);             \
        n1 = (inttype)(u >> 32);                    \
                                                    \
        l -= 64;                                    \
        c += 64;                                    \
        m += 64;                                    \
    }                                               \
    if (l) {                                        \
        unsigned char b[64] = {0};                  \
        long long j;                                \
                                                    \
        for (j = 0; j < l; ++j) b[j] = m[j];        \
        XORBLOCK(b, b);                             \
        ++u;                                        \
        for (j = 0; j < l; ++j) c[j] = b[j];        \
    }                                               \
    ctx->u = u;                                     \
}

_CHACHA_BLOCK(unsigned int, int_chacha20_blocks)
_CHACHA_BLOCK(unsigned long, long_chacha20_blocks)

void chacha20_init(chacha20_ctx *ctx, unsigned char *k, unsigned char *n) {
    ctx->k[0] = unpack32(k     );
    ctx->k[1] = unpack32(k +  4);
    ctx->k[2] = unpack32(k +  8);
    ctx->k[3] = unpack32(k + 12);
    ctx->k[4] = unpack32(k + 16);
    ctx->k[5] = unpack32(k + 20);
    ctx->k[6] = unpack32(k + 24);
    ctx->k[7] = unpack32(k + 28);
    ctx->n[0] = unpack32(n     );
    ctx->n[1] = unpack32(n +  4);
    ctx->u = 0;
}

void chacha20_blocks(chacha20_ctx *ctx, unsigned char *c, const unsigned char *m, unsigned long long l) {
    if (sizeof(int) == 4) {
        int_chacha20_blocks(ctx, c, m, l);
    }
    else {
        long_chacha20_blocks(ctx, c, m, l);
    }
}
