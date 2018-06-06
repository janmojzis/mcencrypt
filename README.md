# mcencrypt
The post-quantum ecryption/decryption tool.</br>
MCENCRYPT is abbreviated from "MCeliece8192128 ENCRYPTion tool".</br>
[![TravisCI status](https://travis-ci.org/janmojzis/mcencrypt.svg?branch=master)](https://travis-ci.org/janmojzis/mcencrypt)

## About
Simple encryption/decryption tool which contains mckeypair,mcencrypt,mcdecrypt.
the tool in inspirated in
[encryption systems https://libpqcrypto.org/command.html](https://libpqcrypto.org/command.html),
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
If decryption fails, mcdecrypt produces an empty output, prints an error message on stderr, and exits 100.

## Crypto
mcencrypt uses post-quantum safe crypto-algorithms:
* **mceliece8192128** public-key algorithm, see [https://classic.mceliece.org](https://classic.mceliece.org)
* **chacha20** symetric cipher
* **poly1305** symetric authenticator
* **SHA512** hash function

## How it works
**mckeypair** creates randomly generated a mceliece8192128 secret key, a corresponding public key,
writes public key on filedescriptor 5 and also writes secret key on filedescriptor 9.
```
14080 BYTES mceliece8192128 secret key
```
```
1357824 BYTES mceliece8192128 public key
```
**mcencrypt** reads public key from filedescriptor 4, reads message from standard input and writes
ciphertext as follows
```
240 BYTES mceliece8192128 ciphertext
n BYTES chacha20 encrypted message
16 BYTES poly1305 authenticator
```
**mcdecrypt** reads secret key from filedescriptor 8, reads ciphertext and writes message on standard output.
If decryption fails, mcdecrypt produces an empty output, prints an error message on stderr, and exits 100.
