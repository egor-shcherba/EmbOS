#include <stdio.h>
#include <stdlib.h>
#include <system/timer.h>

void*
cmd_uptime(void *arg)
{
  (void) arg;
  unsigned time = timer_gettime();

  unsigned s = (time / 1000) % 60;
  unsigned m = (time / 1000) / 60 % 60;
  unsigned h = (time / 1000) / 3600;

  printf("%.2d:%.2d:%.2ds\n", h, m, s);

  return EXIT_SUCCESS;
}
