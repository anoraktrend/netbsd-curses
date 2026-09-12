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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Provide DEFFILEMODE if not already */
#ifndef DEFFILEMODE
#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif

/* Provide emalloc family – NetBSD's <util.h> declares these, our shim must too. */
#include <netbsd_sys/emalloc.h>

/* Provide getprogname() fallback using glibc's program_invocation_name */
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

/* Stub pidfile etc if needed – not used by tic */
#ifndef HAVE_PIDFILE
/* no pidfile needed */
#endif

#endif /* host */

#endif
