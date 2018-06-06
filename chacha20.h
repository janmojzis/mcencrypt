#ifndef _CHACHA20_H____
#define _CHACHA20_H____

#define chacha20_KEYBYTES 32
#define chacha20_NONCEBYTES 8
#define chacha20_BLOCKBYTES 64

typedef struct chacha20_ctx {
    unsigned long k[8];
    unsigned long n[2];
    unsigned long long u;
} chacha20_ctx;

extern void chacha20_init(chacha20_ctx *ctx, unsigned char *, unsigned char *);
extern void chacha20_blocks(chacha20_ctx *ctx, unsigned char *, const unsigned char *, unsigned long long);

#endif
