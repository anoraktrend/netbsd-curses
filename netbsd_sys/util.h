#ifndef PORTABLE_UTIL_H
#define PORTABLE_UTIL_H

/* Portable shim for <util.h> (NetBSD) on Linux.
 * On NetBSD, <util.h> provides getprogname(), pidfile(), etc.
 * On Linux, those are missing. Provide minimal replacements
 * needed by tic/tic.c and infocmp etc.
 */

#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#  if defined(__has_include_next)
#    if __has_include_next(<util.h>)
#      include_next <util.h>
#    endif
#  else
#    error "netbsd_sys/util.h on BSD requires a compiler with __has_include_next (GCC >= 3 or Clang)."
#  endif
#else /* Linux / other */

#include <sys/stat.h>
#include <sys/types.h>
/* Use include_next for headers that are shadowed by netbsd_sys shims
 * (stdio.h, stdlib.h) to avoid recursion when this file is included
 * via netbsd_sys/stdlib.h. */
#if defined(__has_include_next)
# if __has_include_next(<stdio.h>)
#  include_next <stdio.h>
# else
#  include <stdio.h>
# endif
# if __has_include_next(<stdlib.h>)
#  include_next <stdlib.h>
# else
#  include <stdlib.h>
# endif
# if __has_include_next(<string.h>)
#  include_next <string.h>
# else
#  include <string.h>
# endif
#else
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif
#include <stdarg.h>

/* Provide DEFFILEMODE if not already */
#ifndef DEFFILEMODE
#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif

/* Provide emalloc family – NetBSD's <util.h> declares these, our shim must too. */
#include <netbsd_sys/emalloc.h>

/* ------------------------------------------------------------------
 * getprogname / setprogname
 *
 * On Linux with libbsd, <bsd/stdlib.h> already declares
 *   const char *getprogname(void); void setprogname(const char *);
 * with guard LIBBSD_STDLIB_H.  Our fallback must be suppressed when
 * that header is available, regardless of include order.
 *
 * stdlib.h normally defines PORTABLE_HAVE_BSD_STDLIB_H before
 * including this file, but tic/tic.c does `#include <util.h>`
 * directly (via -I./netbsd_sys) which can occur before <stdlib.h>.
 * To make the guard robust, proactively pull in <bsd/stdlib.h> here
 * if it exists and has not already been included.  This mirrors the
 * logic in netbsd_sys/stdlib.h and ensures the guard below sees
 * LIBBSD_STDLIB_H / PORTABLE_HAVE_BSD_STDLIB_H even for util-first
 * includes.
 *
 * On musl (Linux without __GLIBC__) we intentionally avoid libbsd
 * to silence the fgetln linker warning (.gnu.warning.fgetln in
 * libbsd.so).  Our fallback implementations are sufficient.
 * ------------------------------------------------------------------ */
#if defined(__linux__) || defined(__GLIBC__)
#if defined(__linux__) && !defined(__GLIBC__)
/* musl: skip libbsd, use fallback */
#define PORTABLE_MUSL_UTIL 1
#else
# if defined(__has_include)
#  if __has_include(<bsd/stdlib.h>)
#   if !defined(LIBBSD_STDLIB_H) && !defined(PORTABLE_HAVE_BSD_STDLIB_H) && !defined(__BSD_STDLIB_H) && !defined(PORTABLE_MUSL)
#    include <bsd/stdlib.h>
#    if defined(LIBBSD_STDLIB_H) || defined(__BSD_STDLIB_H)
#     define PORTABLE_HAVE_BSD_STDLIB_H 1
#    endif
#   endif
#  endif
# endif
#endif
#endif

/* Provide getprogname() fallback using glibc's program_invocation_name
 * If libbsd already provides getprogname (via <bsd/stdlib.h> which defines
 * LIBBSD_STDLIB_H), skip our static inline to avoid duplicate declaration.
 * Also check __BSD_STDLIB_H (some libbsd versions) and
 * PORTABLE_HAVE_BSD_STDLIB_H from our stdlib wrapper. */
#if !defined(LIBBSD_STDLIB_H) && !defined(PORTABLE_HAVE_BSD_STDLIB_H) && !defined(__BSD_STDLIB_H)
#ifndef __PORTABLE_GETPROGNAME_DECLARED
#define __PORTABLE_GETPROGNAME_DECLARED
#if defined(__GLIBC__) || defined(__linux__)
extern char *program_invocation_short_name;
extern char *program_invocation_name;
static inline const char *getprogname(void) {
    if (program_invocation_short_name && program_invocation_short_name[0])
        return program_invocation_short_name;
    if (program_invocation_name && program_invocation_name[0]) {
        const char *p = program_invocation_name;
        const char *slash = strrchr(p, '/');
        return slash ? slash + 1 : p;
    }
    return "unknown";
}
static inline void setprogname(const char *progname) { (void)progname; }
#else
static inline const char *getprogname(void) { return "tic"; }
static inline void setprogname(const char *p) { (void)p; }
#endif
#endif
#endif

/* Stub pidfile etc if needed – not used by tic */
#ifndef HAVE_PIDFILE
/* no pidfile needed */
#endif

#endif /* host */

#endif
