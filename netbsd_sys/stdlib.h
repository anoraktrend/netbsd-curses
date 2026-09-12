#ifndef PORTABLE_STDLIB_H
#define PORTABLE_STDLIB_H

#if defined(__has_include_next)
#  if __has_include_next(<stdlib.h>)
#    include_next <stdlib.h>
#  endif
#else
#  error "netbsd_sys/stdlib.h requires a compiler with __has_include_next (GCC >= 3 or Clang)."
#endif

/* NetBSD extensions missing on glibc */

/* Centralize getprogname/setprogname in util.h – include it here so
 * consumers of <stdlib.h> keep working without needing both headers. */
#include <netbsd_sys/util.h>

/* mi_vector_hash – provided by nbperf/mi_vector_hash.c */
#ifndef mi_vector_hash
#ifdef __cplusplus
extern "C" {
#endif
void mi_vector_hash(const void *__restrict key, size_t len, uint32_t seed, uint32_t hashes[3]);
#ifdef __cplusplus
}
#endif
#endif

/* Additional NetBSD util helpers that historically lived in <stdlib.h> but we also provide via util.h */
#ifndef HAVE_EMALLOC
/* ensure emalloc declared if util.h not included */
#endif

#endif
