# mcencrypt

The post-quantum ecryption/decryption tool.

## About
Simple encryption/decription tool which contains mckeypair,mcencrypt,mcdecrypt.
The tool in inspirated in
[https://libpqcrypto.org/command.html](https://libpqcrypto.org/command.html)
and uses same commandline interface.

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
* **mceliece8192128** post-quantum safe public-key algorithm, see [https://classic.mceliece.org](https://classic.mceliece.org)
* **chacha20** post-quantum safe symetric cipher
* **poly1305** post-quantum safe symetric authenticator
* ***SHA512** post-quantum safe hash function
