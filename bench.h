#ifndef OPENBLAS_BENCH_H
#define OPENBLAS_BENCH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bench_compat.h"

/* Keep benchmark buffers cache-line aligned without requiring a POSIX API. */
static void *bench_malloc(size_t size)
{
    const size_t alignment = BENCH_CACHELINE;
    void *base = malloc(sizeof(void *) + size + alignment - 1);
    uintptr_t address;
    void **aligned;

    if (base == NULL)
        return NULL;
    address = ((uintptr_t)base + sizeof(void *) + alignment - 1)
              & ~((uintptr_t)alignment - 1);
    aligned = (void **)address;
    aligned[-1] = base;
    return aligned;
}

static void bench_free(void *pointer)
{
    if (pointer != NULL)
        free(((void **)pointer)[-1]);
}

#define malloc(size) bench_malloc(size)
#define free(pointer) bench_free(pointer)

static struct timespec bench_start;
static struct timespec bench_stop;

static void begin(void)
{
    timespec_get(&bench_start, TIME_UTC);
}

static void end(void)
{
    timespec_get(&bench_stop, TIME_UTC);
}

static double getsec(void)
{
    return (double)(bench_stop.tv_sec - bench_start.tv_sec)
         + (double)(bench_stop.tv_nsec - bench_start.tv_nsec) * 1.0e-9;
}

#endif
