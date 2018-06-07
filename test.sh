#!/bin/sh
set -e

log="mckeypair - test if mckeypair handles closed filedescriptor 5"
echo "${log}" 2>&1
if ./mckeypair 9>sk; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mckeypair - test if mckeypair handles closed filedescriptor 9"
echo "${log}" 2>&1
if ./mckeypair 5>pk; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mckeypair - test if mckeypair writes to /dev/null"
echo "${log}" 2>&1
if ! ./mckeypair 5>/dev/null 9>/dev/null; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mckeypair - test if mckeypair rejects to write publickey to /dev/full"
echo "${log}" 2>&1
if [ -e /dev/full ]; then
  if ./mckeypair 5>/dev/full 9>/dev/null; then
    echo "${log} - failed" 2>&1
    exit 1
  fi
else
  echo 'mckeypair: fatal: write publickey failed: out of disk space' 2>&1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mckeypair - test if mckeypair rejects to write secretkey to /dev/full"
echo "${log}" 2>&1
if [ -e /dev/full ]; then
  if ./mckeypair 5>/dev/null 9>/dev/full; then
    echo "${log} - failed" 2>&1
    exit 1
  fi
else
  echo 'mckeypair: fatal: write secretkey failed: out of disk space' 2>&1
fi
echo "${log} - ok" 2>&1
echo 2>&1


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
