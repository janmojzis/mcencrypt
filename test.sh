#!/bin/sh
set -e

#ok test
for i in 10065 1065 165 65; do
  ./mckeypair 5>pk 9>sk
  dd if=/dev/urandom of=data "bs=${i}" count=1 2>/dev/null
  ./mcencrypt 4<pk <data >data.mc8
  ./mcdecrypt 8<sk <data.mc8 >data.new
  if [ "`shasum < data`" = "`shasum < data.new`" ]; then
    echo "test ${i} ok" >&2
  else
    echo "test ${i} failed" >&2
    exit 1
  fi
done

#randomreplace test
for i in `seq 1 100`; do
  ./_randreplace <data.mc8 >data.mc8.corrupted
  if ./mcdecrypt <data.mc8.corrupted 8<sk >data.new 2>log; then
    echo "randomreplace test failed: mcdecrypt decrypted corrupted file: see data.mc8, data.mc8.corrupted" >&2
    exit 1
  fi
done
echo "randomreplace test ok" >&2

#bad secret-key test
./mckeypair 5>pk2 9>sk2
if ./mcdecrypt 8<sk2 <data.mc8 >data.new 2>log; then
  echo "bad secret-key test failed: mcdecrypt decrypted file uder bad secret-key" >&2
  exit 1
else
  echo "bad secret-key test ok" >&2
fi

#short ciphertext test
if ./mcdecrypt 8<sk </dev/null >data.new 2>log; then
  echo "short ciphertext test failed: mcdecrypt accepts too short file" >&2
  exit 1
else
  echo "short ciphertext test ok" >&2
fi

exit 0
