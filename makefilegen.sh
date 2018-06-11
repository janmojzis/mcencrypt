#!/bin/bash

top="`pwd`"
build="`pwd`/build"

rm -rf "${build}"
mkdir -p "${build}"

cp *.c *.h "${build}"
(
  cd "${build}"
  (
    echo "CC=gcc"
    echo "CFLAGS+=-Wall -O3 -funroll-loops -pedantic -Wno-long-long -Wshadow -Wdeclaration-after-statement -Wwrite-strings -Wundef -Wunused-value"
    #echo "LDFLAGS+=-lz"
    echo 

    binaries=""
    objects=""
    for file in `ls *.c`; do
      if grep '^int main(' "${file}" >/dev/null; then
        x=`echo "${file}" | sed 's/\.c$//'`
        binaries="${binaries} ${x}"
      else
        x=`echo "${file}" | sed 's/\.c$/.o/'`
        objects="${objects} ${x}"
      fi
    done

    echo "all: ${binaries}"
    echo 

    for file in `ls *.c`; do
      (
        gcc -MM "${file}"
        echo -e "\t\$(CC) \$(CFLAGS) \$(CPPFLAGS) -c ${file}"
        echo 
      )
    done

    for file in `ls *.c`; do
      if grep '^int main(' "${file}" >/dev/null; then
        x=`echo "${file}" | sed 's/\.c$//'`
        echo "${x}: ${x}.o ${objects}"
        echo -e "\t\$(CC) \$(CFLAGS) \$(CPPFLAGS) \$(LDFLAGS) -o ${x} ${x}.o ${objects}"
        echo 
      fi
    done

    echo "clean:"
    echo -e "\trm -f *.o ${binaries} test.out"
    echo -e "\trm -rf build"
    echo -e "\trm -f badpk badsk data data.mc8 data.mc8.corrupted data.new pk shortdata.mc8 sk"
    echo 

    echo "test: test.sh test.exp test.sk test.data.mc8 ${binaries}"
    echo -e "\tsh test.sh >test.out 2>&1 || { cat test.out; exit 1; }"
    echo -e "\tcmp test.exp test.out || { diff -u test.exp test.out; exit 1; }"
    echo

  ) > Makefile
)
