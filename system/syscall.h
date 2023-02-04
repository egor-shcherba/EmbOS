#ifndef _SYSCALL_H
#define _SYSCALL_H

#define SYS_thread_create   0
#define SYS_thread_yield    1
#define SYS_thread_sleep    2
#define SYS_thread_wakeup   3

int _syscall(int num, void *a1, void *a2, void *a3, void *a4, void *a5);

#endif /* NOT _SYSCALL_H */
