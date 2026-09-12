#ifndef PORTABLE_STDIO_H
#define PORTABLE_STDIO_H

#if defined(__has_include_next)
#  if __has_include_next(<stdio.h>)
#    include_next <stdio.h>
#  endif
#else
#  error "netbsd_sys/stdio.h requires a compiler with __has_include_next (GCC >= 3 or Clang)."
#endif

/* On Linux glibc, fpurge is not in <stdio.h> but __fpurge is in <stdio_ext.h>.
 * __fpurge returns void; provide an int-returning wrapper matching BSD fpurge.
 */
#if defined(__linux__) && !defined(__NetBSD__)
#include <stdio_ext.h>
#ifndef fpurge
static inline int fpurge(FILE *fp) { __fpurge(fp); return 0; }
#endif
#endif

#endif
