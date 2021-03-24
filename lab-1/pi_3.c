#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


pthread_mutex_t lock;
long long N, thread_num;
long double pi;




void* handler(void* ID){
    // calculate thread id
    int id = (int)ID;
    int nums = N/thread_num;


    // calculate count range in this thread
    int start = nums*id;
    int end  = nums*(id+1);
   
    for (int i = start + 1; i <= end; i++){
        int bit = i%2?1:-1;
        // lock to calculate global varibal pi
        pthread_mutex_lock(&lock);
        pi += bit*(1.0/(2*i-1));
        // Unlock
        pthread_mutex_unlock(&lock);

    }

    
}

int main(int args, char* argc[]){
    pi = 0;
    if (args < 2){
        printf("Expected Arguments");
        return -1;
    }

    int N = atoi(argc[1]);
    int thread_num = atoi(argc[2]);

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t*) * thread_num);

    for (int i = 0; i < thread_num; i++){
        pthread_create(&threads[i], NULL, handler, (void*)i);
    }

    for (int i = 0; i < thread_num; i++){
        pthread_join(threads[i], NULL);
    }
    pi = pi*4;
    printf("pi: %.10LF\n",pi);
}