#ifndef PORTABLE_TERMIOS_H
#define PORTABLE_TERMIOS_H

/* Portable shim for <termios.h> – ensures BSD ttydefaults are available on musl. */

#if defined(__has_include_next)
#  if __has_include_next(<termios.h>)
#    include_next <termios.h>
#  endif
#else
#  include <termios.h>
#endif

/* On Linux musl, <termios.h> does not pull in <sys/ttydefaults.h> automatically.
 * Include it explicitly if available. */
#if defined(__has_include)
#  if __has_include(<sys/ttydefaults.h>)
#    include <sys/ttydefaults.h>
#  endif
#endif

/* Fallback BSD defaults if still missing – matches NetBSD/musl-bsd-headers values. */
#ifndef CERASE
# define CERASE 0177
#endif
#ifndef CINTR
# define CINTR 003
#endif
#ifndef CKILL
# define CKILL 025
#endif
#ifndef CQUIT
# define CQUIT 034
#endif
#ifndef CEOF
# define CEOF 004
#endif
#ifndef CSTART
# define CSTART 021
#endif
#ifndef CSTOP
# define CSTOP 023
#endif
#ifndef CSUSP
# define CSUSP 032
#endif
#ifndef CEOL
# define CEOL 0
#endif
#ifndef CEOL2
# define CEOL2 0
#endif
#ifndef CWERASE
# define CWERASE 027
#endif
#ifndef CLNEXT
# define CLNEXT 026
#endif
#ifndef CSTATUS
# define CSTATUS 0
#endif
#ifndef CDISCARD
# define CDISCARD 017
#endif
#ifndef CFLUSH
# define CFLUSH CDISCARD
#endif
#ifndef CRPRNT
# define CRPRNT 022
#endif

#ifndef CTRL
# define CTRL(x) ((x) & 037)
#endif

#endif
