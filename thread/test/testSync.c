#include <stdio.h>

#include <pthread.h>
#include <unistd.h>

struct shared
{
   pthread_mutex_t mut;
   pthread_cond_t synchro;
};

struct data
{
   /* paramètres */
   int nb;
   char const *sid;

   /* contexte partage' */
   struct shared *psh;
};

static void *task_a (void *p)
{
   if (p != NULL)
   {
      struct data *p_data = p;
      struct shared *psh = p_data->psh;
      int i;

      for (i = 0; i < p_data->nb; i++)
      {
         pthread_mutex_lock (&psh->mut);
         printf ("'%s' (%d)\n", p_data->sid, i);
         pthread_cond_signal (&psh->synchro);
         pthread_mutex_unlock (&psh->mut);
         usleep (1000 * 1000);
      }
   }
   return NULL;
}

static void *task_b (void *p)
{
   if (p != NULL)
   {
      struct data *p_data = p;
      struct shared *psh = p_data->psh;
      int i;

      for (i = 0; i < p_data->nb; i++)
      {
         pthread_mutex_lock (&psh->mut);
         pthread_cond_wait (&psh->synchro, &psh->mut);
         printf ("                        "
                 "'%s' (%d)\n", p_data->sid, i);
         pthread_mutex_unlock (&psh->mut);
      }
   }
   return NULL;
}

int main (void)
{
   pthread_t ta;
   pthread_t tb;

   struct shared sh =
   {
      .mut = PTHREAD_MUTEX_INITIALIZER,
      .synchro = PTHREAD_COND_INITIALIZER,
   };

   struct data da =
   {
      .nb = 5,
      .sid = "task A",
      .psh = &sh,
   };

   struct data db =
   {
      .nb = 4,
      .sid = "Task B",
      .psh = &sh,
   };

   puts ("main init");

   pthread_create (&ta, NULL, task_a, &da);
   pthread_create (&tb, NULL, task_b, &db);

   pthread_join (ta, NULL);
   pthread_join (tb, NULL);

   puts ("main end");

   return 0;
}