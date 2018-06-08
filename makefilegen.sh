#!/bin/bash

top="`pwd`"
build="`pwd`/build"

rm -rf "${build}"
mkdir -p "${build}"

cp *.c *.h "${build}"
(
  cd "${build}"
  (
    #echo "CC=gcc"
    echo "CFLAGS+=-Wall -O3"
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
    echo -e "\trm -f *.o ${binaries}"
    echo 

    echo "test: test.sh test.exp ${binaries}"
    echo -e "\tsh test.sh >test.out 2>&1"
    echo -e "\tcmp test.exp test.out"
    echo

  ) > Makefile
)
