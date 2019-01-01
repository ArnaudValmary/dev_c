#include <stdio.h>

#include <pthread.h>

static void *task (void *p_data)
{
   int i;

   for (i = 0; i < 5; i++)
   {
      printf ("Hello world (%d)\n", i);
   }

   (void) p_data;
   return NULL;
}


int main (void)
{
   pthread_t ta;
   pthread_t tb;

   puts ("main init");

   pthread_create (&ta, NULL, task, NULL);
   pthread_create (&tb, NULL, task, NULL);

   pthread_join (ta, NULL);
   pthread_join (tb, NULL);

   puts ("main end");

   return 0;
}
