# mcencrypt
MCENCRYPT is abbreviated from "MCeliece8192128 ENCRYPTion tool".</br>
[![TravisCI status](https://travis-ci.org/janmojzis/mcencrypt.svg?branch=master)](https://travis-ci.org/janmojzis/mcencrypt)

## About
Post-quantum public-key encryption/decryption tool which uses public key to encrypt
and secret key to decrypt message. It contains mckeypair,mcencrypt,mcdecrypt.
The tool in inspirated in
[https://libpqcrypto.org/command.html (encryption systems)](https://libpqcrypto.org/command.html),
uses same commandline interface.

**mckeypair** is used to generate a key pair:
```
mckeypair 5>publickey 9>secretkey
```
**mcencrypt** is used to encrypt a message:
```
mcencrypt <message 4<publickey >ciphertext
```
**mcdecrypt** is used to decrypt a ciphertext and recover the original message:
```
mcdecrypt <ciphertext 8<secretkey >message
```

## How it works
**mckeypair** creates randomly generated a mceliece8192128 secret key, a corresponding public key,
writes public key on filedescriptor 5 and also writes secret key on filedescriptor 9.
```
14080 BYTES mceliece8192128 secret key
```
```
1357824 BYTES mceliece8192128 public key
```
**mcencrypt** reads mceliece8192128 public key from filedescriptor 4,
uses KEM (key encapsulation mode) to compute symetric session key and ciphertext.
Then reads n-BYTES message from standard input, uses chacha20-poly1305 to encrypt and authenticate the message
and writes ciphertext on standard output.
```
240 BYTES mceliece8192128 ciphertext
n BYTES chacha20 encrypted message
16 BYTES poly1305 authenticator
```
**mcdecrypt** reads mceliece8192128 secret key from filedescriptor 8, 
uses ciphertext/secret key to compute session key.
Then reads ciphertext,encrypted message from standard input, uses chacha20-poly1305 to verify,decrypt
and write the decrypted message on standard output.
If decryption fails, mcdecrypt produces an empty output, prints an error message on stderr, and exits 100.

## Crypto
mcencrypt uses post-quantum safe crypto-algorithms:
* **mceliece8192128** public-key code-based crypto-system, see [mceliece-20171129.pdf](https://classic.mceliece.org/nist/mceliece-20171129.pdf)
* **chacha20-poly1305** symetric encryption/authentication, see [chacha-20080128.pdf](https://cr.yp.to/chacha/chacha-20080128.pdf), [poly1305-20041101.pdf](https://cr.yp.to/mac/poly1305-20041101.pdf)

## Example
```
#create key-pair
umask 077
mckeypair 5>pk 9>sk
```
```
#encrypt tarball
(cd somewhere; tar -vjcf - *) | mcencrypt 4<pk >data.tar.bz2.mc8
```
```
#decrypt tarball
mcdecrypt 8<sk <data.tar.bz2.mc8 | (cd somewhere; tar -vjxf -)
```

## Implementation notes
**mceliece8192128** public-domain implementation taken from [https://classic.mceliece.org/software.html](https://classic.mceliece.org/software.html), files
benes.c, benes.h, bm.c, bm.h, controlbits.c, controlbits.h, decrypt.c, decrypt.h, encrypt.c, encrypt.h, gf.c, gf.h, operations.c, params.h, root.c, root.h, sk_gen.c, sk_gen.h, synd.c, synd.h, transpose.c, transpose.h, util.c, util.h and modified to use SHA512 instead of SHA3256.</br>
**poly1305** public-domain implementation based on [https://github.com/floodyberry/poly1305-donna](https://github.com/floodyberry/poly1305-donna)

## 1TB test

computer
```
cat /proc/cpuinfo | grep 'model name' | sort | uniq -c
     32 model name	: Intel(R) Xeon(R) CPU E5-2630L v3 @ 1.80GHz
```
```
df -h | grep sda5
/dev/sda5        11T  1.6T  9.4T  14% /
```

create keys
```
time -p mckeypair 5>pk 9>sk
real 1.44
user 1.24
sys 0.20
```

create 1TB file
```
dd if=/dev/urandom of=data bs=1 count=1099511627789 2>/dev/null
```

encrypt
```
time -p mcencrypt <data 4<pk >data.mc8
real 8650.60
user 4030.29
sys 1003.61
```

decrypt
```
time -p mcdecrypt <data.mc8 8<sk >data.new
real 12071.66
user 5544.74
sys 1394.20
```

compare checksums
```
test "`shasum < data`" = "`shasum < data.new`" 
```

result
```
encryption 121MB/s
decryption 86MB/s
```
