#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

long long N;
long double pi;


void handler(){
   for (int i = 1; i <= N; i++){
        int bit = i%2?1:-1;
        pi += bit*(1.0/(2*i-1));
    }
}

int main(int args, char* argc[]){
    pi = 0;
    if (args < 1){
        printf("Expected Arguments");
        return -1;
    }

    N = atoi(argc[1]);

    // Calculate pi
    handler();
    pi = pi*4;
    printf("pi: %.10LF\n",pi);
}