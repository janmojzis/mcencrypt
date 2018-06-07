#!/bin/sh
set -e

#ok test
for i in 65 165 1065 10065; do
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

#bad secret-key test
./mckeypair 5>pk2 9>sk2
if ./mcdecrypt 8<sk2 <data.mc8 >data.new 2>log; then
  echo "bad secret-key test failed:" >&2
  cat log >&2
  exit
else
  echo "bad secret-key test ok" >&2
fi

#short ciphertext test
if ./mcdecrypt 8<sk </dev/null >data.new 2>log; then
  echo "short ciphertext test failed:" >&2
  cat log >&2
  exit 1
else
  echo "short ciphertext test ok" >&2
fi

exit 0
