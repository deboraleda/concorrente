#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t *mutex;
sem_t *sem1;

struct thread_param {
  long t_id;
  int random_s;
  int prev_s;
};

int ended_threads_count = 0;
int t_number = 0;

void *phase_one(void *t){
    struct thread_param *param = (struct thread_param*) t;
    long id = (long)param->t_id;
    int time_sleep = rand() % 5;
    printf("PHASE ONE: Thread %ld sleeping for %d seconds\n", id, time_sleep);
    sleep(time_sleep);
    int s = rand() % 10;
    param->random_s = s;
    printf("PHASE ONE: Thread %ld DONE! With random_s = %d\n", id, s);

    sem_wait(sem1);
    ended_threads_count++;
    if(ended_threads_count == t_number){
        sem_post(mutex);
    }
    sem_post(sem1);

    pthread_exit(NULL);
}

void *phase_two(void *t){
    struct thread_param *param = (struct thread_param*) t;
    int time_sleep = (int)param->prev_s;
    printf("PHASE TWO: Thread %ld with prev_s = %d\n", param->t_id, param->prev_s);
    sleep(time_sleep);
    printf("PHASE TWO: Thread %ld DONE!\n", param->t_id);

    sem_wait(sem1);
    ended_threads_count++;
    if (ended_threads_count == t_number){
        sem_post(mutex);
    }
    sem_post(sem1);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int n = atoi(argv[1]);
    t_number = n;

    pthread_t threads[n];
    struct thread_param params[n];

    mutex = sem_open("/mutex", O_CREAT, 0644, 0);
    sem1 = sem_open("/sem1", O_CREAT, 0644, 1);

    printf("-----Phase One-----\n");

    for (int i = 0; i < n; i++){
        params[i].t_id = i;
        pthread_create(&threads[i], NULL, phase_one, (void *)&params[i]);
    }

    sem_wait(mutex);

    printf("-----Phase Two-----\n");

    ended_threads_count = 0;

    for (int i = 0; i < n; i++){
        int prev_thread_id = (i + n - 1) % n;
        int s = params[prev_thread_id].random_s;
        params[i].prev_s = s;
        pthread_create(&threads[i], NULL, phase_two, (void *)&params[i]);
    }

    sem_close(mutex);
    sem_unlink("/mutex");
    sem_close(sem1);
    sem_unlink("/sem1");
    pthread_exit(NULL);
}
