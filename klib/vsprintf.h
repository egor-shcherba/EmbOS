#ifndef _VSPRINTF_H
#define _VSPRINTF_H

#include <stdarg.h>

int sprintf(char *buf, const char *format, ...);
int vsprintf(char *buf, const char *format, va_list ap);

#endif /* NOT _VSPRINTF_H */
