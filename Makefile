# Standalone OpenBLAS benchmark suite
CC ?= cc
PKG_CONFIG ?= pkg-config

OPENBLAS_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags openblas)
OPENBLAS_LIBS ?= $(shell $(PKG_CONFIG) --libs openblas)

CPPFLAGS += $(OPENBLAS_CFLAGS)
CFLAGS ?= -O2
CFLAGS += -std=c11
LDLIBS += $(OPENBLAS_LIBS)

BLAS_SOURCES := gemm gemv trmm trsm axpy
BLAS_BENCHMARKS := $(foreach op,$(BLAS_SOURCES),s$(op) d$(op) c$(op) z$(op))
LAPACK_BENCHMARKS := linpack cholesky gesv geev getri potrf
BENCHMARKS := $(BLAS_BENCHMARKS) $(LAPACK_BENCHMARKS)

.PHONY: all clean check-openblas
all: check-openblas $(BENCHMARKS)

check-openblas:
	@printf '%s\n' '$(OPENBLAS_CFLAGS) $(OPENBLAS_LIBS)' | grep -q '[^[:space:]]' || { \
		echo 'OpenBLAS was not found. Install its development package or set OPENBLAS_CFLAGS and OPENBLAS_LIBS.' >&2; \
		exit 1; \
	}

define real_single_template
s$(1): $(1).c bench.h bench_compat.h
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) $$< $$(LDFLAGS) $$(LDLIBS) -o $$@
endef
define real_double_template
d$(1): $(1).c bench.h bench_compat.h
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -DDOUBLE $$< $$(LDFLAGS) $$(LDLIBS) -o $$@
endef
define complex_single_template
c$(1): $(1).c bench.h bench_compat.h
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -DCOMPLEX $$< $$(LDFLAGS) $$(LDLIBS) -o $$@
endef
define complex_double_template
z$(1): $(1).c bench.h bench_compat.h
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -DCOMPLEX -DDOUBLE $$< $$(LDFLAGS) $$(LDLIBS) -o $$@
endef

$(foreach op,$(BLAS_SOURCES),$(eval $(call real_single_template,$(op))))
$(foreach op,$(BLAS_SOURCES),$(eval $(call real_double_template,$(op))))
$(foreach op,$(BLAS_SOURCES),$(eval $(call complex_single_template,$(op))))
$(foreach op,$(BLAS_SOURCES),$(eval $(call complex_double_template,$(op))))

# The traditional unprefixed LAPACK programs use double precision.
$(LAPACK_BENCHMARKS): %: %.c bench.h bench_compat.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -DDOUBLE $< $(LDFLAGS) $(LDLIBS) -o $@

clean:
	$(RM) $(BENCHMARKS) $(addsuffix .exe,$(BENCHMARKS))
