# mcencrypt

The post-quantum ecryption/decryption tool.

## About
Simple encryption/decription tool which contains tools mckeypair,mcencrypt,mcdecrypt.
### mckeypair
mckeypair 5>publickey 9>secretkey
### mcencrypt
mcencrypt <message 4<publickey >ciphertext
### mcdecrypt
mcdecrypt <ciphertext 8<secretkey >message

The tool in inspirated in
[https://libpqcrypto.org/command.html](https://libpqcrypto.org/command.html)
and uses same commandline interface.

