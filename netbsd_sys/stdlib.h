#ifndef PORTABLE_STDLIB_H
#define PORTABLE_STDLIB_H

#if defined(__has_include_next)
#  if __has_include_next(<stdlib.h>)
#    include_next <stdlib.h>
#  endif
#else
#  error "netbsd_sys/stdlib.h requires a compiler with __has_include_next (GCC >= 3 or Clang)."
#endif

/* ------------------------------------------------------------------
 * Portable BSD arc4random / reallocarray / etc.
 *
 * - glibc >= 2.36 with _DEFAULT_SOURCE already declares arc4random in
 *   <stdlib.h>, so nothing to do.
 * - musl and glibc < 2.36 provide them via libbsd <bsd/stdlib.h>.
 * - If libbsd is not available, provide inline fallbacks using
 *   random()/realloc() so the build still succeeds (Werror).
 * ------------------------------------------------------------------ */
#if defined(__linux__) || defined(__GLIBC__)

# if defined(__has_include)
#  if __has_include(<bsd/stdlib.h>)
#   include <bsd/stdlib.h>
#   define PORTABLE_HAVE_BSD_STDLIB_H 1
#  endif
# endif

/* Fallback when neither system nor libbsd provides arc4random.
 * Detect via PORTABLE_HAVE_BSD_STDLIB_H and glibc version.
 */
# if !defined(PORTABLE_HAVE_BSD_STDLIB_H)
   /* No bsd header available – need fallback if system didn't provide. */
#  if !defined(__GLIBC__) || !__GLIBC_PREREQ(2, 36)
#   include <stdint.h>
#   include <stddef.h>
     /* errno for reallocarray fallback */
#   include <errno.h>
#   ifndef arc4random
static inline uint32_t arc4random(void) {
    /* random() is seeded via srandom() elsewhere; for nbperf we only
     * need non-cryptographic variability, so this is sufficient. */
    return (uint32_t)random();
}
#   endif
#   ifndef arc4random_buf
static inline void arc4random_buf(void *buf, size_t n) {
    uint8_t *p = (uint8_t *)buf;
    for (size_t i = 0; i < n; i++)
        p[i] = (uint8_t)(random() & 0xFF);
}
#   endif
#   ifndef arc4random_uniform
static inline uint32_t arc4random_uniform(uint32_t upper_bound) {
    if (upper_bound == 0) return 0;
    return arc4random() % upper_bound;
}
#   endif
     /* reallocarray fallback – glibc < 2.26 and musl without libbsd */
#   ifndef reallocarray
static inline void *reallocarray(void *ptr, size_t nmemb, size_t size) {
    if (size != 0 && nmemb > (size_t)-1 / size) {
        errno = ENOMEM;
        return NULL;
    }
    return realloc(ptr, nmemb * size);
}
#   endif
#  endif /* !glibc 2.36 */
# endif /* !PORTABLE_HAVE_BSD_STDLIB_H */

#endif /* __linux__ || __GLIBC__ */

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
