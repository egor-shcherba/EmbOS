#ifndef _PIC_H
#define _PIC_H

void pic_init(void);

void irq_set_handler(int irq_line, void (*handler)(void));
void irq_enable(int irq_line);
void irq_disable(int irq_line);

#endif /* NOT _PIC_H */
