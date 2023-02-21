#ifndef _TIMER_H
#define _TIMER_H

void timer_tick(void);
void timer_init(void);

unsigned int timer_gettime(void);

int sys_settimer(unsigned int mseconds);

#endif /* _TIMER_H*/
