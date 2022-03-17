#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


struct thread_param {
  long t_id;
  int random_s;
  int s_prev_thread;
};

void *sleep_thread(void *t){
    struct thread_param *param = (struct thread_param*) t;
    long id = (long)param->t_id;
    int time_sleep = rand() % 5;
    printf("Thread %ld sleeping for %d seconds\n", id, time_sleep);
    sleep(time_sleep);
    printf("Thread %ld DONE!\n", id);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int n = atoi(argv[1]);

    pthread_t threads[n];
    struct thread_param params[n];

    for (int i = 0; i < n; i++){
        params[i].t_id = i;
        pthread_create(&threads[i], NULL, sleep_thread, (void *)&params[i]);
    }

    for (int i = 0; i < n; i++) {
 		pthread_join(threads[i], NULL);
 	}

    printf("Thread mãe - o valor de n foi: %d\n", n);
}
