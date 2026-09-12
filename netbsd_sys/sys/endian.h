#ifndef PORTABLE_SYS_ENDIAN_H
#define PORTABLE_SYS_ENDIAN_H

/* On Linux, glibc provides <endian.h> but not <sys/endian.h>.
 * NetBSD code expects <sys/endian.h> with le32toh, le32dec etc.
 * Delegate to portable netbsd_sys/endian.h which already handles
 * both <endian.h> and byteorder conversions.
 */

#include <netbsd_sys/endian.h>

#endif
