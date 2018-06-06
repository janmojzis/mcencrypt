#ifndef _POLY1305_H____
#define _POLY1305_H____

#define poly1305_BYTES 16
#define poly1305_KEYBYTES 32

typedef struct poly1305_ctx {
    unsigned long h[5];
    unsigned long r[5];
    unsigned long s[5];
    unsigned long pad[4];
} poly1305_ctx;

extern void poly1305_init(poly1305_ctx *, unsigned const char *);
extern void poly1305_blocks(poly1305_ctx *, const unsigned char *, unsigned long long);
extern void poly1305_final(poly1305_ctx *, unsigned char *);
extern void poly1305(unsigned char *, const unsigned char *, unsigned long long, const unsigned char *);
extern int poly1305_verify(const unsigned char *, const unsigned char *, unsigned long long, const unsigned char *);

#endif
