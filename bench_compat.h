#ifndef OPENBLAS_BENCH_COMPAT_H
#define OPENBLAS_BENCH_COMPAT_H

/* Preserve the benchmark's command-line precision selector.  Some installed
   openblas_config.h variants undefine DOUBLE as part of exposing the library
   configuration, but that must not change which benchmark was requested. */
#ifdef DOUBLE
#define BENCH_DOUBLE 1
#endif

/* The installed configuration header supplies blasint and BLASFUNC, so the
   benchmark follows the LP64 or ILP64 ABI selected by the OpenBLAS package.
   OpenBLAS's public f77blas.h declares both BLAS and LAPACK entry points. */
#include <openblas_config.h>
#include <f77blas.h>

/* FLOAT and all routine selection are derived from the saved selector rather
   than from macros the installed headers may define or undefine. */
#ifdef BENCH_DOUBLE
#define FLOAT double
#else
#define FLOAT float
#endif

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
