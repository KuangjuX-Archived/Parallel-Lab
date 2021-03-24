#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

long long N;
long double pi;
long long m = 0, n = 0;
int BITS = 99999;


void handler(){
    int nums = N;
    // calcuate probability N times
    srand(time(NULL));
    for (int i = 0; i < nums; i++){
        float x = rand() % (BITS + 1) / (float)(BITS + 1);
        float y = rand() % (BITS + 1) / (float)(BITS + 1);
        n++;
        if (x*x + y*y <= 1)m++;
    }

}

int main(int args, char* argc[]){
    if (args < 1){
        printf("Expected Arguments.\n");
        return -1;
    }
    pi = 0;
    N = atoi(argc[1]);

    handler();

    pi = 4*((m*1.0)/n);
    printf("pi: %.10LF\n",pi);
}