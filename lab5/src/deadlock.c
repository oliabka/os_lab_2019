/*Mutex Deadlock: This condition occurs when a mutex is applied but then not "unlocked". 
This causes program execution to halt indefinitely. 
It can also be caused by poor application of mutexes or joins. 
Be careful when applying two or more mutexes to a section of code. 
If the first pthread_mutex_lock is applied and the second pthread_mutex_lock fails
 due to another thread applying a mutex, the first mutex may eventually lock all other threads 
 from accessing data including the thread which holds the second mutex. 
 The threads may wait indefinitely for the resource to become free causing a deadlock. 
 It is best to test and if failure occurs, free the resources and stall before retrying.
 
 Deadlock — это программа, в которой все параллельные процессы ожидают друг друга. 
 В этом состоянии программа никогда не восстановится без вмешательства извне.
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
int common = 0;

 void function1(int *pnum_times)
    {
       unsigned long k;
       int work;
       pthread_mutex_lock(&lock1);           // Execution step 1
       pthread_mutex_lock(&lock2);           // Execution step 3 DEADLOCK!!!
       
                
       printf("doing one thing\n");
       work = *pnum_times;
       printf("counter = %d\n", work);
       work++; /* increment, but not write */
       for (k = 0; k < 500000; k++)
         ;                 /* long cycle */
       *pnum_times = work; /* write back */


       pthread_mutex_lock(&lock2);
       pthread_mutex_lock(&lock1);
    } 

    void function2(int *pnum_times)
    {
       unsigned long k;
       int work;
       pthread_mutex_lock(&lock2);           // Execution step 2
       pthread_mutex_lock(&lock1);


       printf("doing one thing\n");
       work = *pnum_times;
       printf("counter = %d\n", work);
       work++; /* increment, but not write */
       for (k = 0; k < 500000; k++)
         ;                 /* long cycle */
       *pnum_times = work; /* write back */

       
       pthread_mutex_lock(&lock1);
       pthread_mutex_lock(&lock2);
    } 
  
    int main()
    {
       pthread_t thread1, thread2;

       pthread_create(&thread1, NULL, (void*)function1, (void *)&common);
       pthread_create(&thread2, NULL, (void*)function1, (void *)&common);
       if (pthread_join(thread1, NULL) != 0) 
       {
            perror("pthread_join");
            exit(1);
       }
       if (pthread_join(thread2, NULL) != 0) 
       {
            perror("pthread_join");
            exit(1);
       }
       return 0;
    }