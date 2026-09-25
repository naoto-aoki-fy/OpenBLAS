#ifndef TEST_OPENBLAS_CONFIG_H
#define TEST_OPENBLAS_CONFIG_H

/* Reproduce installed configurations that consume the internal selector. */
#undef DOUBLE

typedef int blasint;
#define BLASFUNC(name) name##_
#define OPENBLAS_L1_DATA_LINESIZE 64

#endif
