#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

long long int n,t;
long double pi;
pthread_mutex_t lock;
const long double TimeCovert = 1e6;


void *thread_compute(void* ID){
    int id = (int) ID;

    int length;
    length = n/t;
    int start,end;
    start = id*length;
    end = (id+1)*length;

    long double count = 0;
    for(int i = start+1; i <= end; i++){
        count += (4/((1+((i+0.5)/n)*((i+0.5)/n))))*(1.0/n);
    }

    // lock to add count to pi
    pthread_mutex_lock(&lock);
    
    pi += count;

    pthread_mutex_unlock(&lock);
}

int main(int args, char* argc[]){
    pi=0;

    if(args < 2){
        printf("Excepted Arguments.\n");
        return -1;
    }

    n = atoi(argc[1]);
    t = atoi(argc[2]);
    
   
    pthread_t* threads;
    threads = (pthread_t*)malloc(sizeof(pthread_t)*t);
    int i;
    for(i=0;i<t;i++){
        pthread_create(&threads[i],NULL,thread_compute,(void*)i);
    }

    for(i=0;i<t;i++){
        pthread_join(threads[i],NULL);
    }

    printf("pi: %.10LF\n",pi);

    return 0;
    
}