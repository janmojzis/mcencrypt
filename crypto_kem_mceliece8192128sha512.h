#ifndef crypto_kem_mceliece8192128sha512_H
#define crypto_kem_mceliece8192128sha512_H

#define crypto_kem_mceliece8192128sha512_SECRETKEYBYTES 14080
#define crypto_kem_mceliece8192128sha512_PUBLICKEYBYTES 1357824
#define crypto_kem_mceliece8192128sha512_CIPHERTEXTBYTES 240
#define crypto_kem_mceliece8192128sha512_BYTES 64
 
extern int crypto_kem_mceliece8192128sha512_keypair(unsigned char *, unsigned char *);
extern int crypto_kem_mceliece8192128sha512_enc(unsigned char *, unsigned char *, const unsigned char *);
extern int crypto_kem_mceliece8192128sha512_dec(unsigned char *, const unsigned char *, const unsigned char *);

#endif
