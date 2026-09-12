#ifndef PORTABLE_TTYENT_H
#define PORTABLE_TTYENT_H

/* Portable shim for <ttyent.h> – BSD-specific, missing on musl and some Linux.
 * Provide minimal stubs so tset/term.c can compile on Linux.
 */

#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#  if defined(__has_include_next)
#    if __has_include_next(<ttyent.h>)
#      include_next <ttyent.h>
#    endif
#  else
#    include <ttyent.h>
#  endif
#else /* Linux / other */
#  if defined(__has_include_next)
#    if __has_include_next(<ttyent.h>)
#      include_next <ttyent.h>
#    else
/* Stub for Linux – tset only uses getttynam() to map tty name to type. */
#      ifndef _TTYENT_STUB
#      define _TTYENT_STUB
struct ttyent {
    char *ty_name;
    char *ty_getty;
    char *ty_type;
    int ty_status;
    char *ty_window;
    char *ty_comment;
};
#      endif
static inline struct ttyent *getttyent(void) { return NULL; }
static inline struct ttyent *getttynam(const char *name) { (void)name; return NULL; }
static inline struct ttyent *getttyent_r(void) { return NULL; }
static inline int setttyent(void) { return 0; }
static inline int endttyent(void) { return 0; }
#    endif
#  else
#    if defined(__has_include) && __has_include(<ttyent.h>)
#      include <ttyent.h>
#    else
struct ttyent {
    char *ty_name;
    char *ty_getty;
    char *ty_type;
    int ty_status;
    char *ty_window;
    char *ty_comment;
};
static inline struct ttyent *getttyent(void) { return NULL; }
static inline struct ttyent *getttynam(const char *name) { (void)name; return NULL; }
#    endif
#  endif
#endif

#endif
