#include <stdio.h>

#include <pthread.h>
#include <unistd.h>

static void *task_a (void *p_data)
{
   int i;

   for (i = 0; i < 5; i++)
   {
      printf ("Hello world A (%d)\n", i);
      usleep(1000);
   }

   (void) p_data;
   return NULL;
}

static void *task_b (void *p_data)
{
   int i;

   for (i = 0; i < 7; i++)
   {
      printf ("Hello world B    (%d)\n", i);
      usleep(1000);
   }
   (void) p_data;
   return NULL;
}

int main (void)
{
   pthread_t ta;
   pthread_t tb;

   puts ("main init");

   pthread_create (&ta, NULL, task_a, NULL);
   pthread_create (&tb, NULL, task_b, NULL);

#if 1
   pthread_join (ta, NULL);
   pthread_join (tb, NULL);
#endif

   puts ("main end");

   return 0;
}
