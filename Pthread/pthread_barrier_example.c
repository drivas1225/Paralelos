/* L-18 MCS 572 Mon 3 Oct 2016 : pthread_barrier_example.c
 * The code below illustrates the use of a pthread barrier.
 * All threads generate a random number (rolling a 6-sided die)
 * and sleep as many seconds as the value from the die.
 * Their sleeping time is recorded in a global array.
 * The barrier ensures the global array is filled up completely
 * before all threads print its values to screen. */

#include <stdlib.h>
#include <stdio.h> 
#include <pthread.h> 

int size;   /* size equals the number of threads */
int *data;  /* shared data, as many ints as size */
pthread_barrier_t our_barrier; /* to synchronize */

void *fun ( void *args )
{
   int *id = (int*) args;
   int r = 1 + (rand() % 6);
   int k;
   char strd[size+1];

   sleep(r);
   printf("Thread %d has slept %d seconds ...\n", *id, r);
   data[*id] = r;

   pthread_barrier_wait(&our_barrier);
   
   for(k=0; k<size; k++) strd[k] = '0' + ((char) data[k]);
   strd[size] = '\0';

   printf("Thread %d has data : %s\n", *id, strd);
}

int main ( int argc, char* argv[] )
{
   printf("Give the number of threads : "); scanf("%d", &size);
   data = (int*) calloc(size, sizeof(int));
   {
      pthread_t t[size];
      pthread_attr_t a;
      int id[size], i;

      pthread_barrier_init(&our_barrier, NULL, size);

      for(i=0; i<size; i++)
      {
         id[i] = i;
         pthread_attr_init(&a);
         if(pthread_create(&t[i], &a, fun, (void*)&id[i]) != 0)
            printf("Unable to create thread %d!\n", i);
      }
      printf("Created %d threads ...\n", size);
      for(i=0; i<size; i++) pthread_join(t[i], NULL);

      pthread_barrier_destroy(&our_barrier);
   }
   return 0;
}
