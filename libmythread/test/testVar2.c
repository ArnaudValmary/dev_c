#include <stdio.h>

#include <pthread.h>

struct data
{
   char const *id;
   int n;
};

static void *task (void *p_data)
{
   if (p_data != NULL)
   {
      struct data *p = p_data;
      int i;

      for (i = 0; i < p->n; i++)
      {
         printf ("Hello world %s (%d)\n", p->id, i);
      }
   }

   return NULL;
}

int main (void)
{
   pthread_t ta;
   pthread_t tb;
   struct data data_a = { "A", 5 };
   struct data data_b = { "B", 7 };

   puts ("main init");

   pthread_create (&ta, NULL, task, &data_a);
   pthread_create (&tb, NULL, task, &data_b);

   pthread_join (ta, NULL);
   pthread_join (tb, NULL);

   puts ("main end");

   return 0;
}
