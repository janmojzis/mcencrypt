CC=gcc
CFLAGS+=-Wall -O3 -funroll-loops -pedantic -Wno-long-long -Wshadow -Wdeclaration-after-statement -Wwrite-strings -Wundef -Wunused-value

all:  _randreplace mcdecrypt mcencrypt mckeypair

_randreplace.o: _randreplace.c memreadall.h die.h writeall.h fsyncfd.h \
  limits.h randombytes.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c _randreplace.c

benes.o: benes.c util.h gf.h transpose.h params.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c benes.c

bm.o: bm.c params.h gf.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c bm.c

chacha20.o: chacha20.c chacha20.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c chacha20.c

checkfd.o: checkfd.c checkfd.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c checkfd.c

controlbits.o: controlbits.c controlbits.h randombytes.h params.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c controlbits.c

crypto_hash_sha512.o: crypto_hash_sha512.c crypto_hash_sha512.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c crypto_hash_sha512.c

decrypt.o: decrypt.c decrypt.h params.h benes.h gf.h util.h synd.h root.h \
  bm.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c decrypt.c

die.o: die.c e.h writeall.h die.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c die.c

e.o: e.c e.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c e.c

encrypt.o: encrypt.c encrypt.h util.h gf.h params.h randombytes.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c encrypt.c

fsyncfd.o: fsyncfd.c fsyncfd.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c fsyncfd.c

gf.o: gf.c gf.h params.h randombytes.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c gf.c

limits.o: limits.c randombytes.h limits.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c limits.c

mcdecrypt.o: mcdecrypt.c writeall.h checkfd.h memreadall.h readblock.h \
  die.h max.h limits.h fsyncfd.h randombytes.h chacha20.h poly1305.h \
  crypto_kem_mceliece8192128sha512.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c mcdecrypt.c

mcencrypt.o: mcencrypt.c writeall.h checkfd.h readblock.h fsyncfd.h die.h \
  max.h limits.h randombytes.h chacha20.h poly1305.h \
  crypto_kem_mceliece8192128sha512.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c mcencrypt.c

mckeypair.o: mckeypair.c writeall.h checkfd.h fsyncfd.h die.h limits.h \
  randombytes.h crypto_kem_mceliece8192128sha512.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c mckeypair.c

memreadall.o: memreadall.c max.h memreadall.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c memreadall.c

operations.o: operations.c crypto_hash_sha512.h \
  crypto_kem_mceliece8192128sha512.h params.h sk_gen.h gf.h pk_gen.h \
  encrypt.h decrypt.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c operations.c

pk_gen.o: pk_gen.c pk_gen.h gf.h params.h benes.h root.h util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c pk_gen.c

poly1305.o: poly1305.c poly1305.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c poly1305.c

randombytes.o: randombytes.c randombytes.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c randombytes.c

readblock.o: readblock.c readblock.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c readblock.c

root.o: root.c params.h gf.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c root.c

sk_gen.o: sk_gen.c sk_gen.h gf.h randombytes.h controlbits.h params.h \
  util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c sk_gen.c

synd.o: synd.c synd.h gf.h params.h root.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c synd.c

transpose.o: transpose.c transpose.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c transpose.c

util.o: util.c util.h gf.h params.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c util.c

writeall.o: writeall.c writeall.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c writeall.c

_randreplace: _randreplace.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o _randreplace _randreplace.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o

mcdecrypt: mcdecrypt.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o mcdecrypt mcdecrypt.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o

mcencrypt: mcencrypt.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o mcencrypt mcencrypt.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o

mckeypair: mckeypair.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o mckeypair mckeypair.o  benes.o bm.o chacha20.o checkfd.o controlbits.o crypto_hash_sha512.o decrypt.o die.o e.o encrypt.o fsyncfd.o gf.o limits.o memreadall.o operations.o pk_gen.o poly1305.o randombytes.o readblock.o root.o sk_gen.o synd.o transpose.o util.o writeall.o

clean:
	rm -f *.o  _randreplace mcdecrypt mcencrypt mckeypair test.out

test: test.sh test.exp  _randreplace mcdecrypt mcencrypt mckeypair
	sh test.sh >test.out 2>&1 || { cat test.out; exit 1; }
	cmp test.exp test.out || { diff -u test.exp test.out; exit 1; }

