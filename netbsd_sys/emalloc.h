#ifndef EMALLOC_H
#define EMALLOC_H

/* replacement functions for emalloc() and friends used in netbsd code */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
/* Avoid recursion through netbsd_sys/stdlib.h wrapper:
 * use include_next to get the real system header when CPPFLAGS
 * shadows <stdlib.h> with netbsd_sys/stdlib.h. */
#if defined(__has_include_next)
# if __has_include_next(<stdlib.h>)
#  include_next <stdlib.h>
# else
#  include <stdlib.h>
# endif
#else
# include <stdlib.h>
#endif
#if defined(__has_include_next)
# if __has_include_next(<stdio.h>)
#  include_next <stdio.h>
# else
#  include <stdio.h>
# endif
#else
# include <stdio.h>
#endif
#include <stdarg.h>
#if defined(__has_include_next)
# if __has_include_next(<string.h>)
#  include_next <string.h>
# else
#  include <string.h>
# endif
#else
# include <string.h>
#endif

static void mallerr(void) {
	dprintf(2, "error: out of memory\n");
	abort();
}
static inline void* emalloc(size_t x) {
	void *p=malloc(x);
	if(!p) mallerr();
	return p;
}
static inline void* ecalloc(size_t x, size_t y) {
	void *p=calloc(x, y);
	if(!p) mallerr();
	return p;
}
static inline void* erealloc(void *x, size_t y) {
	void *p=realloc(x, y);
	if(!p) mallerr();
	return p;
}
static inline char* estrdup(const char* x) {
	char *p = strdup(x);
	if(!p) mallerr();
	return p;
}
static int easprintf(char **s, const char *fmt, ...) {
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vasprintf(s, fmt, ap);
	va_end(ap);
	if(ret == -1) mallerr();
	return ret;
}

#endif
