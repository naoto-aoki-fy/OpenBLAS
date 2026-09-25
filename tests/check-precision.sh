#!/bin/sh
set -eu

cd "$(dirname "$0")/.."

CC=${CC:-cc}
tmp=${TMPDIR:-/tmp}/openblas-bench-precision-$$
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
mkdir -p "$tmp"

check_variant() {
    name=$1
    symbol=$2
    scalar_size=$3
    shift 3

    "$CC" -E -P -Itests/mock-installed "$@" gemm.c >"$tmp/$name.i"
    grep -Eq "[[:space:]]${symbol}_[[:space:]]*\(" "$tmp/$name.i"

    cat >"$tmp/$name-size.c" <<EOF
#include "bench_compat.h"
_Static_assert(sizeof(FLOAT) == $scalar_size, "wrong FLOAT precision");
int main(void) { return 0; }
EOF
    "$CC" -std=c11 -I. -Itests/mock-installed "$@" \
        -c "$tmp/$name-size.c" -o "$tmp/$name-size.o"
}

check_variant sgemm sgemm 'sizeof(float)'
check_variant dgemm dgemm 'sizeof(double)' -DDOUBLE
check_variant cgemm cgemm 'sizeof(float)' -DCOMPLEX
check_variant zgemm zgemm 'sizeof(double)' -DCOMPLEX -DDOUBLE

printf '%s\n' 'precision selector regression checks passed'
