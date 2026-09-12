#ifndef PORTABLE_SYS_CDEFS_H
#define PORTABLE_SYS_CDEFS_H

/* Wrapper for <sys/cdefs.h> on Linux.
 * Try to include the system's <sys/cdefs.h> first via include_next,
 * then supplement with NetBSD portable definitions.
 * This restores __RCSID, __BEGIN_DECLS, __printflike etc on glibc.
 */

#if defined(__has_include_next)
#  if __has_include_next(<sys/cdefs.h>)
#    include_next <sys/cdefs.h>
#  endif
#else
#  error "netbsd_sys/sys/cdefs.h requires a compiler with __has_include_next (GCC >= 3 or Clang). Remove -I netbsd_sys from CPPFLAGS or use a supported compiler."
#endif

/* Pull in portable NetBSD shims */
#include <netbsd_sys/cdefs.h>

/* Provide __va_list if code expects it (old portable fallback) */
#ifndef __va_list
#include <stdarg.h>
#define __va_list va_list
#endif

/* Ensure __RCSID etc are always available */
#ifndef __RCSID
#define __RCSID(x)
#endif
#ifndef __COPYRIGHT
#define __COPYRIGHT(x)
#endif
#ifndef __FBSDID
#define __FBSDID(x)
#endif

#ifndef __BEGIN_DECLS
#ifdef __cplusplus
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif
#endif

/* Provide BSD-only macros if system header lacked them */
#ifndef __predict_true
#if __GNUC__ >= 3
#define __predict_true(exp)  __builtin_expect((exp) != 0, 1)
#define __predict_false(exp) __builtin_expect((exp) != 0, 0)
#else
#define __predict_true(exp) (exp)
#define __predict_false(exp) (exp)
#endif
#endif

#ifndef __arraycount
#define __arraycount(a) (sizeof(a) / sizeof(*(a)))
#endif

#endif /* PORTABLE_SYS_CDEFS_H */
