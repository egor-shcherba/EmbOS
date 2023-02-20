#ifndef _VSSCANF_H
#define _VSSCANF_H

#include <stdarg.h>

int sscanf(char *buf, const char *format, ...);
int vsscanf(char *buf, const char *format, va_list ap);

#endif /* NOT _VSSCANF_H */
