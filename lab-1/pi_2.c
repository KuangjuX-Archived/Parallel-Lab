#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t lock;
long long N, thread_num;
long double pi;
long long m = 0, n = 0;
int BITS = 99999;


void* handler(){
    int nums = N/thread_num;
    // calcuate probability N times
    int local_n = 0;
    int local_m = 0;
    srand(time(NULL));
    for (int i = 0; i < nums; i++){
        float x = rand() % (BITS + 1) / (float)(BITS + 1);
        float y = rand() % (BITS + 1) / (float)(BITS + 1);
        local_n++;
        if (x*x + y*y <= 1)local_m++;
    }

    // lock
    pthread_mutex_lock(&lock);
    n += local_n;
    m += local_m;
    // unlock
    pthread_mutex_unlock(&lock);

}

int main(int args, char* argc[]){
    if (args < 2){
        printf("Expected Arguments.\n");
        return -1;
    }
    pi = 0;
    N = atoi(argc[1]);
    thread_num = atoi(argc[2]);

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t*) * thread_num);

    for (int i = 0; i < thread_num; i++){
        pthread_create(&threads[i], NULL, handler, NULL);
    }

    for (int i = 0; i < thread_num; i++){
        pthread_join(threads[i], NULL);
    }
    pi = 4*((m*1.0)/n);
  

    printf("pi: %.10LF\n",pi);
}