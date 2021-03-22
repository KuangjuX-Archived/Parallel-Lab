#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

pthread_mutex_t lock;
long long N, thread_num;
long double pi;
long long m = 0, n = 0;
int BITS = 99999;


union FloatRand
{
    struct 
    {
        unsigned long Frac:23;
        unsigned long Exp:8;
        unsigned long Signed:1;
    } BitArea;
    float Value;
    unsigned long Binary; /* for debug only */
};



void* handler(){
    int nums = N/thread_num;
    // calcuate probability N times
    srand(time(NULL));
    for (int i = 0; i < nums; i++){
        float x = rand() % (BITS + 1) / (float)(BITS + 1);
        float y = rand() % (BITS + 1) / (float)(BITS + 1);
       
        // debug
        // printf("x: %f, y:%f\n", x, y);

        // lock to modify global 
        pthread_mutex_lock(&lock);
        n++;
        if (x*x + y*y <= 1)m++;
        // unlock 
        pthread_mutex_unlock(&lock);
    }

}

int main(){
    pi = 0;
    printf("Please enter N and Thread Nums: \n");
    scanf("%lld %lld",&N, &thread_num);

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t*) * thread_num);

    for (int i = 0; i < thread_num; i++){
        pthread_create(&threads[i], NULL, handler, NULL);
    }

    for (int i = 0; i < thread_num; i++){
        pthread_join(threads[i], NULL);
    }
    pi = 4*((m*1.0)/n);
    printf("m: %lld n:%lld \n", m, n);
    printf("pi: %.10LF\n",pi);
}