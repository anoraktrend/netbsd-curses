#ifndef PORTABLE_SEARCH_H
#define PORTABLE_SEARCH_H

#if defined(__has_include_next)
#  if __has_include_next(<search.h>)
#    include_next <search.h>
#  endif
#else
#  error "netbsd_sys/search.h requires a compiler with __has_include_next (GCC >= 3 or Clang)."
#endif

/* NetBSD extensions: hdestroy1 / hdestroy1_r take free callbacks;
 * glibc only has hdestroy() / hdestroy_r().  These shims are intentionally
 * invented for portability – the _r variant is a no-op (no reentrant
 * table free available) and hdestroy1 simply delegates to hdestroy().
 * Guarded on !defined(hdestroy1) so a future glibc that adds it will win.
 */
#ifndef hdestroy1
#include <stddef.h>
static inline void hdestroy1(void (*freekey)(void *), void (*freedata)(void *)) {
    (void)freekey;
    (void)freedata;
    hdestroy();
}
static inline void hdestroy1_r(struct hsearch_data *htab, void (*freekey)(void *), void (*freedata)(void *)) {
    (void)htab; (void)freekey; (void)freedata;
    /* No portable fallback for reentrant version – just no-op */
}
#endif

#endif
