# mcencrypt

The post-quantum ecryption/decryption tool.

## About
Simple encryption/decription tool which contains mckeypair,mcencrypt,mcdecrypt.
### mckeypair
mckeypair is used to generate a key pair:
```
mckeypair 5>publickey 9>secretkey
```
### mcencrypt
mcencrypt is used to encrypt a message:
```
mcencrypt <message 4<publickey >ciphertext
```
### mcdecrypt
mcdecrypt is used to decrypt a ciphertext and recover the original message:
```
mcdecrypt <ciphertext 8<secretkey >message
```

The tool in inspirated in
[https://libpqcrypto.org/command.html](https://libpqcrypto.org/command.html)
and uses same commandline interface.

