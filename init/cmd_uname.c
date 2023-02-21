#include <stdio.h>

void*
cmd_uname(void *arg)
{
  (void) arg;

  printf("EmbOS\n");

  return 0;
}
