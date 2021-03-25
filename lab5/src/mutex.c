/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *  Modified by A.Kostin
 ********************************************************
 * mutex.c
 *
 * Simple multi-threaded example with a mutex lock.
 */

 /*  Для компиляции -lpthread */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void do_one_thing(int *);
void do_another_thing(int *);
void do_wrap_up(int);
int common = 0; /* A shared variable for two threads */
int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main() {
  pthread_t thread1, thread2;

/* Create independent threads each of which will execute function */
  if (pthread_create(&thread1, NULL, (void *)do_one_thing,
			  (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)do_another_thing,
                     (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

 /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  do_wrap_up(common);

  return 0;
}

/* Mutexes are used to prevent data inconsistencies due to race conditions. 
 A race condition often occurs when two or more threads need to perform operations on the same memory area,
 but the results of computations depends on the order in which these operations are performed. 
 Mutexes are used for serializing shared resources. 
 Anytime a global resource is accessed by more than one thread the resource should have a Mutex associated with it. 
 One can apply a mutex to protect a segment of memory ("critical region") from other threads. 
 Mutexes can be applied only to threads in a single process and do not work between processes as do semaphores.*/

void do_one_thing(int *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    // pthread_mutex_lock(&mut);
    printf("doing one thing\n");
    work = *pnum_times;
    printf("counter = %d\n", work);
    work++; /* increment, but not write */
    for (k = 0; k < 500000; k++)
      ;                 /* long cycle */
    *pnum_times = work; /* write back */
	// pthread_mutex_unlock(&mut);
  }
}

void do_another_thing(int *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    // pthread_mutex_lock(&mut);
    printf("doing another thing\n");
    work = *pnum_times;
    printf("counter = %d\n", work);
    work++; /* increment, but not write */
    for (k = 0; k < 500000; k++)
      ;                 /* long cycle */
    *pnum_times = work; /* write back */
    // pthread_mutex_unlock(&mut);
  }
}

void do_wrap_up(int counter) {
  int total;
  printf("All done, counter = %d\n", counter);
}
