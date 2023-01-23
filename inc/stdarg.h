#ifndef _STDARG_H
#define _STDARG_H

typedef __builtin_va_list va_list;

#define va_start(ap, last)  __builtint_va_start(ap, last)
#define va_arg(ap, type)    __builtint_va_arg(ap, type)
#define va_end(ap)          __builtint_va_end(ap)
#define va_copy(dest, scr)  __builtint_va_copy(dest, src)

#endif /* NOT _STDARG_H */
