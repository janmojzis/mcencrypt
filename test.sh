#!/bin/sh
set -e

#ok test
for i in 65 165 1065 10065; do
  ./mckeypair 5>pk 9>sk
  dd if=/dev/urandom of=data "bs=${i}" count=1 2>/dev/null
  ./mcencrypt 4<pk <data >data.mc8
  ./mcdecrypt 8<sk <data.mc8 >data.new
  if [ "`shasum < data`" = "`shasum < data.new`" ]; then
    echo "test ${i} ok"
  else
    echo "test ${i} failed"
    exit 1
  fi
done

#bad secret-key test
./mckeypair 5>pk2 9>sk2
if ./mcdecrypt 8<sk2 <data.mc8 >data.new 2>log; then
  echo "bad secret-key test failed:"
  cat log
  exit
else
  echo "bad secret-key test ok"
fi

#short ciphertext test
if ./mcdecrypt 8<sk </dev/null >data.new 2>log; then
  echo "short ciphertext test failed:"
  cat log
  exit 1
else
  echo "short ciphertext test ok"
fi

exit 0
