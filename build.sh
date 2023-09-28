#!/usr/bin/env bash

CFLAGS="-Isrc -Wall -pedantic -Wextra -Werror -g -O0"
# CFLAGS="-Isrc -O3"
LDFLAGS="-ldl"

echo "CC jule"
gcc -o jule src/driver.c ${CFLAGS} ${LDFLAGS} || exit $?

for f in packages/*.c; do
    SO=$(dirname $f)/$(basename $f .c).so
    echo "CC ${SO}"
    gcc -shared -fPIC -o ${SO} $f ${CFLAGS} ${LDFLAGS} -lm || exit $?
done
