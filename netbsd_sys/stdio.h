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

/* ------------------------------------------------------------------
 * fgetln replacement via getline for musl
 *
 * musl's linker emits:
 *   warning: The fgetln() function cannot be safely ported,
 *            use getline(3) instead
 * whenever the external fgetln symbol is referenced (via libbsd or
 * musl's own .gnu.warning.fgetln).  NetBSD's fileio.c history used
 * fgetln; modern code already uses getline, but to guarantee the
 * warning never appears we provide an inline fgetln that wraps
 * POSIX getline(3).  This satisfies any stray fgetln reference
 * without pulling the external symbol.
 *
 * Semantics: match BSD fgetln: returns pointer to internal buffer
 * valid until next call, with *lenp set to line length (including
 * newline if present).  We use static buffer managed by getline.
 *
 * Implementation: use a differently-named static inline and macro
 * to avoid colliding with the system's extern declaration
 *   char *fgetln(FILE *, size_t *);
 * which on musl is provided with a .gnu.warning.  Redefining the
 * function as static would be a conflicting declaration, so we
 * provide portable_fgetln and macro-replace fgetln.
 * ------------------------------------------------------------------ */
#if defined(__linux__) && !defined(__NetBSD__) && !defined(__GLIBC__)
#include <stdlib.h>
#include <string.h>
#ifndef _PORTABLE_FGETLN_DEFINED
#define _PORTABLE_FGETLN_DEFINED
static inline char *portable_fgetln(FILE *fp, size_t *lenp) {
    static char *portable_fgetln_buf = NULL;
    static size_t portable_fgetln_cap = 0;
    ssize_t n;

    if (lenp == NULL)
        return NULL;
    if (fp == NULL) {
        *lenp = 0;
        return NULL;
    }
    n = getline(&portable_fgetln_buf, &portable_fgetln_cap, fp);
    if (n == -1) {
        *lenp = 0;
        return NULL;
    }
    *lenp = (size_t)n;
    return portable_fgetln_buf;
}
/* Replace any subsequent fgetln use with our portable version.
 * This ensures no external reference is generated. */
#ifdef fgetln
#undef fgetln
#endif
#define fgetln portable_fgetln
#endif
#endif

#endif
