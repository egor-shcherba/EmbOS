#include <stdlib.h>

void*
cmd_ud2(void *arg)
{
  (void) arg;

  __asm__ volatile ("ud2");

  return EXIT_SUCCESS;
}
