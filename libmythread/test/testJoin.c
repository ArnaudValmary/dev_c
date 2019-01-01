#include <stdio.h>

#include <pthread.h>
#include <libmythread/libmythread.h>

void printThreadInfos(void) {
   printf("- mythreadIsMainThread = %d / TID = %ld\n", mythreadIsMainThread(), mythreadGetTID());
   fflush(stdin);
}

static void *task_a (void *p_data)
{
   printf("Hello world A\n");
   fflush(stdin);
   printThreadInfos();

   (void) p_data;
   return NULL;
}

static void *task_b (void *p_data)
{
   printf("Hello world B\n");
   fflush(stdin);
   printThreadInfos();

   (void) p_data;
   return NULL;
}

int main (void)
{
   pthread_t ta;
   pthread_t tb;

   printf("main init\n");
   fflush(stdin);
   printThreadInfos();

   pthread_create (&ta, NULL, task_a, NULL);
   pthread_create (&tb, NULL, task_b, NULL);

#if 1
   pthread_join (ta, NULL);
   pthread_join (tb, NULL);
#endif

   printf("main end\n");
   fflush(stdin);

   return 0;
}
