#ifndef _EXCEPTION_H
#define _EXCEPTION_H

extern void exc_divide_error(void);
extern void exc_debug(void);
extern void exc_nmi(void);
extern void exc_breakpoint(void);
extern void exc_overflow(void);
extern void exc_boundrange(void);
extern void exc_inv_opcode(void);
extern void exc_no_dev(void);
extern void exc_double_fault(void);     /* error code*/

extern void exc_inv_tss(void);          /* error code */
extern void exc_seg_not_present(void);  /* error code */
extern void exc_ss(void);               /* error code */
extern void exc_general_prot(void);     /* error code */
extern void exc_page_fault(void);

extern void exc_x87(void);              /* 16 */
extern void exc_aligned_check(void);    /* error code */
extern void exc_machine_check(void);
extern void exc_simd(void);

#endif /* NOT _EXCEPTION_H */
