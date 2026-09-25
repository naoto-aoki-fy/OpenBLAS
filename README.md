# Standalone OpenBLAS benchmarks

A small GNU make benchmark suite for an **externally installed** OpenBLAS. It
contains the upstream benchmark programs but does not contain or build the
OpenBLAS implementation.

## Requirements

- GNU make and a C11 compiler
- OpenBLAS development headers and library
- `pkg-config` (called `pkgconf` by some package managers)

On Debian/Ubuntu, for example:

```sh
sudo apt install make gcc pkg-config libopenblas-dev
make
```

OpenBLAS is discovered with `pkg-config --cflags openblas` and
`pkg-config --libs openblas`. Normal make variables are respected. For an
installation without a pkg-config file, specify its public include and library
flags directly:

```sh
make OPENBLAS_CFLAGS="-I/opt/openblas/include" \
     OPENBLAS_LIBS="-L/opt/openblas/lib -lopenblas"
```

`CC`, `CPPFLAGS`, `CFLAGS`, `LDFLAGS`, `LDLIBS`, and `PKG_CONFIG` may also be
overridden in the usual way.

## Benchmarks

The default target builds single, double, complex-single, and complex-double
versions of GEMM, GEMV, TRMM, TRSM, and AXPY (`sgemm` through `zaxpy`). It also
builds the double-precision LAPACK-oriented `linpack`, `cholesky`, `gesv`,
`geev`, `getri`, and `potrf` programs.

Most programs accept `from`, `to`, and `step` dimensions. Keep validation runs
short, for example:

```sh
./sgemm 10 20 10
./dgemm 10 20 10
./cgemm 10 20 10
./zgemm 10 20 10
./potrf 10 20 10
```

Run `make clean` to remove every generated executable.

Run `make check` to exercise the precision-selection regression test. The test
uses mock installed headers that undefine `DOUBLE` and verifies all four GEMM
precision variants without requiring an OpenBLAS installation.

## MSYS2 UCRT64

From an MSYS2 UCRT64 shell:

```sh
pacman -S --needed make mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-pkgconf mingw-w64-ucrt-x86_64-openblas
make
```

No `/ucrt64` path or pthread flag is hardcoded; the active UCRT64 pkg-config
installation supplies the correct compiler and linker flags.

## ABI and portability

`bench_compat.h` includes OpenBLAS's installed `openblas_config.h` and `f77blas.h` (which also declares the
LAPACK entry points). In particular, `blasint` comes from the installed configuration,
so LP64 and ILP64 packages use their matching integer ABI. The local header only
provides benchmark scalar/type macros, cache-line alignment, and small utility
macros that the original private `common.h` used to provide.
