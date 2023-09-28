#!/usr/bin/env bash

CFLAGS="-Wall -pedantic -Wextra -Werror -g -O0 -ldl"
# CFLAGS="-O3"

echo "CC jule"
gcc -o jule driver.c ${CFLAGS} || exit $?

for f in packages/*.c; do
    SO=$(dirname $f)/$(basename $f .c).so
    echo "CC ${SO}"
    gcc -shared -fPIC -o ${SO} $f -I. ${CFLAGS} -lm || exit $?
done
