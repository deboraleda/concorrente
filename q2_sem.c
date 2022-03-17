#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t sem;
static sem_t mutex;

struct thread_param {
  long t_id;
};

int ended_threads_count = 0;
int t_number = 0;

void *sleep_thread(void *t){
    struct thread_param *param = (struct thread_param*) t;
    long id = (long)param->t_id;
    int time_sleep = rand() % 5;
    printf("Thread %ld sleeping for %d seconds\n", id, time_sleep);
    sleep(time_sleep);
    printf("Thread %ld DONE!\n", id);

    sem_wait(&mutex);
    ended_threads_count++;
    if(ended_threads_count == t_number){
        printf("Todas as threads filhas finalizaram, liberando thread-mãe\n");
        sem_post(&sem);
    }
    sem_post(&mutex);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    srand(time(NULL));
    int n = atoi(argv[1]);

    t_number = n;

    int mutex_ret = sem_init(&mutex, 0, 1);
    if (mutex_ret == -1){
        printf("mutex nãoo iniciado %d", mutex_ret);
        exit(-1);
    }

    int sem_value = 0;
    int ret = sem_init(&sem, 0, sem_value);
    if (ret == -1){
        printf("sem não iniciado %d", ret);
        exit(-1);
    }

    pthread_t threads[n];
    struct thread_param params[n];    

    for (int i = 0; i < n; i++){
        params[i].t_id = i;
        pthread_create(&threads[i], NULL, sleep_thread, (void *)&params[i]);
    }

    sem_wait(&sem);

    printf("Thread mãe - o valor de n foi: %d\n", n);
    sem_destroy(&sem);
    sem_destroy(&mutex);

}
