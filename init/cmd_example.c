 #include <stdio.h>
 #include <stdlib.h>
 #include <thread.h>
 #include <unistd.h>

 static void*
 thread_example(void *arg)
 {
   (void) arg;

   printf("thread example\n");
   sleep(2);
   printf("thread end\n");

   return (void*) EXIT_SUCCESS;
 }

 void*
 cmd_example(void *arg)
 {
  (void) arg;
  
  struct thread *t1;
  thread_create(&t1, "EXAMPLE THREAD", &thread_example, NULL);

  return (void*) EXIT_SUCCESS;
 }
