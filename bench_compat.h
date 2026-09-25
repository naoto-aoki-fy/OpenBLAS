#ifndef OPENBLAS_BENCH_COMPAT_H
#define OPENBLAS_BENCH_COMPAT_H

/* Select the scalar representation before f77blas.h, whose declarations use
   OpenBLAS's FLOATRET compatibility macro. */
#ifdef DOUBLE
#define FLOAT double
#else
#define FLOAT float
#endif
/* The installed configuration header supplies blasint and BLASFUNC, so the
   benchmark follows the LP64 or ILP64 ABI selected by the OpenBLAS package.
   OpenBLAS's public f77blas.h declares both BLAS and LAPACK entry points. */
#include <openblas_config.h>
#include <f77blas.h>

#ifndef OPENBLAS_L1_DATA_LINESIZE
#define OPENBLAS_L1_DATA_LINESIZE 64
#endif
#define BENCH_CACHELINE OPENBLAS_L1_DATA_LINESIZE

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#define TOUPPER(c) do { if ((c) >= 'a' && (c) <= 'z') (c) -= 'a' - 'A'; } while (0)

#ifdef COMPLEX
#define COMPSIZE 2
#else
#define COMPSIZE 1
#endif

#endif
