#ifndef PORTABLE_SYS_QUEUE_H
#define PORTABLE_SYS_QUEUE_H

/* Portable wrapper: prefer system <sys/queue.h> if it exists,
 * otherwise fall back to bundled NetBSD queue.h.
 */

#if defined(__has_include_next)
#  if __has_include_next(<sys/queue.h>)
#    include_next <sys/queue.h>
#  else
#    include <netbsd_sys/queue.h>
#  endif
#else
/* No include_next support – try system header, fallback */
#  if defined(__linux__) || defined(__GLIBC__)
#    include <netbsd_sys/queue.h>
#  else
#    include <netbsd_sys/queue.h>
#  endif
#endif

#endif
