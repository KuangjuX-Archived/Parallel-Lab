#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

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
    // debug
    printf("start: %d, end: %d\n", start, end);
    for (int i = start + 1; i <= end; i++){
        // lock to calculate global varibal pi
        int bit = i%2?1:-1;
        printf("i: %d, bit: %d, num:%f \n", i, bit, bit*(1.0/(2*i-1)));
        pthread_mutex_lock(&lock);
        pi += bit*(1.0/(2*i-1));
        pthread_mutex_unlock(&lock);

    }

    
}

int main(){
    pi = 0;
    printf("Please enter N and Thread Nums: \n");
    scanf("%lld %lld",&N, &thread_num);

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