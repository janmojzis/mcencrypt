#!/bin/sh
set -e

LANG=C
export LANG

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

log="mckeypair - test if mckeypair writes to /dev/null"
echo "${log}" 2>&1
if ! ./mckeypair 5>/dev/null 9>/dev/null; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mckeypair - test if mckeypair writes to regular file"
echo "${log}" 2>&1
if ! ./mckeypair 5>pk 9>sk; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

#create data file
dd if=/dev/urandom of=data bs=1025 count=1 2>/dev/null
#create bad pk
dd if=pk of=badpk bs=1 count=1025 2>/dev/null
#create bad sk
dd if=sk of=badsk bs=1 count=1025 2>/dev/null

log="mcencrypt - test if mcencrypt handles closed filedescriptor 4"
echo "${log}" 2>&1
if ./mcencrypt < data >data.mc8; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcencrypt - test if mcencrypt rejects bad pk"
echo "${log}" 2>&1
if ./mcencrypt 4<badpk < data >data.mc8; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcencrypt - test if mcencrypt rejects bad pk from /dev/null"
echo "${log}" 2>&1
if ./mcencrypt 4</dev/null < data >data.mc8; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcencrypt - test if mcencrypt accepts pk from /dev/zero"
echo "${log}" 2>&1
if ! ./mcencrypt 4</dev/zero < data >data.mc8; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcencrypt - test if mcencrypt rejects to write to /dev/full"
echo "${log}" 2>&1
if [ -e /dev/full ]; then
  if ./mcencrypt 4<pk < data >/dev/full; then
    echo "${log} - failed" 2>&1
    exit 1
  fi
else
  echo 'mcencrypt: fatal: write ciphertext failed: out of disk space'  2>&1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcencrypt - test if mcencrypt encrypts the message"
echo "${log}" 2>&1
if ! ./mcencrypt 4<pk < data >data.mc8; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

#create short data.mc8
dd if=sk of=shortdata.mc8 bs=1 count=255 2>/dev/null

log="mcdecrypt - test if mcdecrypt handles closed filedescriptor 8"
echo "${log}" 2>&1
if ./mcdecrypt < data.mc8 >data.new; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcdecrypt - test if mdencrypt rejects bad sk"
echo "${log}" 2>&1
if ./mcdecrypt 8<badsk < data.mc8 >data.new; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcdecrypt - test if mdencrypt rejects bad sk from /dev/null"
echo "${log}" 2>&1
if ./mcdecrypt 8</dev/null < data.mc8 >data.new; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

#log="mcdecrypt - test if mcdecrypt accepts pk from /dev/zero"
#echo "${log}" 2>&1
#if ! ./mcdecrypt 8</dev/zero < data.mc8 >data.new; then
#  echo "${log} - failed" 2>&1
#  exit 1
#fi
#echo "${log} - ok" 2>&1
#echo 2>&1

log="mcdecrypt - test if mcdecrypt rejects to write to /dev/full"
echo "${log}" 2>&1
if [ -e /dev/full ]; then
  if ./mcdecrypt 8<sk < data.mc8 >/dev/full; then
    echo "${log} - failed" 2>&1
    exit 1
  fi
else
  echo 'mcdecrypt: fatal: write message failed: out of disk space'  2>&1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mcdecrypt - test if mdencrypt rejects short ciphertext"
echo "${log}" 2>&1
if ./mcdecrypt 8<sk < shortdata.mc8 >data.new; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1


log="mdencrypt - test if mcdecrypt decrypts the ciphertext"
echo "${log}" 2>&1
if ! ./mcdecrypt 8<sk < data.mc8 >data.new; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mdencrypt - test if data and data.new match"
echo "${log}" 2>&1
if [ "`shasum < data`" != "`shasum < data.new`" ]; then
  echo "${log} - failed" 2>&1
  exit 1
fi
echo "${log} - ok" 2>&1
echo 2>&1

log="mdencrypt - test if mcdecrypt rejects bad ciphertext"
echo "${log}" 2>&1
for i in `seq 1 10`; do
  ./_randreplace <data.mc8 >data.mc8.corrupted
  if ./mcdecrypt <data.mc8.corrupted 8<sk >data.new; then
    echo "${log} - failed" 2>&1
    exit 1
  fi
done
echo "${log} - ok" 2>&1
echo 2>&1

log="mcencrypt/mdencrypt - test if mcencrypt/mcdecrypt encrypts/decrypts"
echo "${log}" 2>&1
for i in 10065 1065 165 65; do
  dd if=/dev/urandom of=data bs=1 "count=${i}" 2>/dev/null
  ./mcencrypt 4<pk <data >data.mc8
  ./mcdecrypt 8<sk <data.mc8 >data.new
  if [ "`shasum < data`" != "`shasum < data.new`" ]; then
    echo "${log} - failed" 2>&1
    exit 1
  fi
done
echo "${log} - ok" 2>&1
echo 2>&1

exit 0
