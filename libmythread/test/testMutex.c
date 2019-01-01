#include <stdio.h>

#include <pthread.h>

struct shared
{
   int data;
   pthread_mutex_t mut;
};

struct data
{
   /* paramètres */
   int nb;
   char const *sid;

   /* contexte partage' */
   struct shared *psh;
};

static void *task_w (void *p)
{
   if (p != NULL)
   {
      struct data *p_data = p;
      int i;

      for (i = 0; i < p_data->nb; i++)
      {

/* debut de la zone critique */
         pthread_mutex_lock (&p_data->psh->mut);
         {
            int x = p_data->psh->data;
            x++;
            p_data->psh->data = x;
         }
         printf ("'%s' (%d) data <- %d\n", p_data->sid, i, p_data->psh->data);
         pthread_mutex_unlock (&p_data->psh->mut);
/* fin de la zone critique */

      }
   }
   return NULL;
}

static void *task_r (void *p)
{
   if (p != NULL)
   {
      struct data *p_data = p;
      int i;

      for (i = 0; i < p_data->nb; i++)
      {
/* debut de la zone critique */
         pthread_mutex_lock (&p_data->psh->mut);
         printf ("                        "
               "'%s' (%d) data == %d\n", p_data->sid, i, p_data->psh->data);
         pthread_mutex_unlock (&p_data->psh->mut);
/* fin de la zone critique */
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
      .data = 0,
      .mut = PTHREAD_MUTEX_INITIALIZER,
   };

   struct data da =
   {
      .nb = 5,
      .sid = "Writer",
      .psh = &sh,
   };

   struct data db =
   {
      .nb = 7,
      .sid = "Reader",
      .psh = &sh,
   };

   puts ("main init");

   pthread_create (&ta, NULL, task_w, &da);
   pthread_create (&tb, NULL, task_r, &db);

   pthread_join (ta, NULL);
   pthread_join (tb, NULL);

   puts ("main end");

   return 0;
}
