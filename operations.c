#include "crypto_hash_sha512.h"
#include "crypto_kem_mceliece8192128sha512.h"

#include "params.h"
#include "sk_gen.h"
#include "pk_gen.h"
#include "encrypt.h"
#include "decrypt.h"

#include <string.h>

int crypto_kem_mceliece8192128sha512_enc(
       unsigned char *c,
       unsigned char *key,
       const unsigned char *pk
)
{
	unsigned char two_e[ 1 + SYS_N/8 ] = {2};
	unsigned char *e = two_e + 1;
	unsigned char one_ec[ 1 + SYS_N/8 + (SYND_BYTES + 32) ] = {1};
	unsigned char conf[64];
    long long i;

	/* // */

	encrypt(c, pk, e);

	crypto_hash_sha512(conf, two_e, sizeof(two_e)); 
    for (i = 0; i < 32; ++i) c[SYND_BYTES + i] = conf[i];

	memcpy(one_ec + 1,e,SYS_N/8);
	memcpy(one_ec + 1 + SYS_N/8,c,SYND_BYTES + 32);

	crypto_hash_sha512(key, one_ec, sizeof one_ec);

	return 0;
}

int crypto_kem_mceliece8192128sha512_dec(
       unsigned char *key,
       const unsigned char *c,
       const unsigned char *sk
)
{
	int i;

	unsigned char ret_confirm = 0;
	unsigned char ret_decrypt = 0;

	uint16_t m;

	unsigned char conf[64];
	unsigned char two_e[ 1 + SYS_N/8 ] = {2};
	unsigned char *e = two_e + 1;
	unsigned char preimage[ 1 + SYS_N/8 + (SYND_BYTES + 32) ];

	/* // */

	ret_decrypt = decrypt(e, sk + SYS_N/8, c);

	crypto_hash_sha512(conf, two_e, sizeof(two_e)); 

	for (i = 0; i < 32; i++)
		ret_confirm |= conf[i] ^ c[SYND_BYTES + i];

	m = ret_decrypt | ret_confirm;
	m -= 1;
	m >>= 8;

	{
		unsigned char *x = preimage;
		*x++ = (~m & 0) |  (m & 1);
		for (i = 0; i < SYS_N/8; i++) 
			*x++ = (~m & sk[i]) | (m & e[i]);
		for (i = 0; i < SYND_BYTES + 32; i++) 
			*x++ = c[i];
	}

	crypto_hash_sha512(key, preimage, sizeof(preimage)); 

	return 0;
}

int crypto_kem_mceliece8192128sha512_keypair
(
       unsigned char *pk,
       unsigned char *sk 
)
{
	while (1)
	{
		sk_gen(sk);

		if (pk_gen(pk, sk + SYS_N/8) == 0)
			break;
	}

	return 0;
}

