#include <stdio.h>

#include <pthread.h>

static void *task_a (void *p_data)
{
   puts ("Hello world A");

   (void) p_data;
   return NULL;
}

static void *task_b (void *p_data)
{
   puts ("Hello world B");

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
