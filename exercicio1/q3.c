#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
pthread_barrier_t barrier;
sem_t mutex;
 
int n;
int *pointer_random_s;
 
struct thread_params {
   int tid;
};
 
void *thread_func(void *t){
   struct thread_params *param = (struct thread_params*) t;
   int tid = (int) param->tid;
  
   //Phase 1
   int time_sleep = rand() % 5;
   printf("PHASE ONE: Thread %d sleeping for %d seconds\n", tid, time_sleep);
   sleep(time_sleep);
   int s = rand() % 10;
   printf("PHASE ONE: Thread %d DONE! With random_s = %d\n", tid, s);
 
   sem_wait(&mutex);
   pointer_random_s[tid] = s;
   sem_post(&mutex);
 
   pthread_barrier_wait(&barrier);
 
   //Phase 2
   int prev_id = (tid + n - 1) % n;
   sem_wait(&mutex);
   int prev_s = pointer_random_s[prev_id];
   sem_post(&mutex);
   printf("PHASE TWO: Thread %d sleeping for %d seconds\n", tid, prev_s);
   sleep(prev_s);
   printf("PHASE TWO: Thread %d DONE\n", tid);
 
   pthread_exit(NULL);
}
 
int main(int argc, char *argv[]){
   srand(time(NULL));
   n = atoi(argv[1]);
 
   int random_s[n];
   pointer_random_s = random_s;
 
   pthread_t threads[n];
   struct thread_params params[n];
 
   pthread_barrier_init(&barrier, NULL, n);
   sem_init(&mutex, 0, 1);
 
   for (int i = 0; i < n; i++){
       params[i].tid = i;
       pthread_create(&threads[i], NULL, thread_func, (void *)&params[i]);
   }
 
   pthread_barrier_destroy(&barrier);
   sem_destroy(&mutex);
   pthread_exit(NULL);
}
