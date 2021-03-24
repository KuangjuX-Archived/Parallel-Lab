#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

long long int n;
long double pi;



void thread_compute(){
    int start,end;
    start = 0;
    end = n;
    
    for(int i=start+1; i<=end; i++){
        pi += (4/((1+((i+0.5)/n)*((i+0.5)/n))))*(1.0/n);
    }
}

int main(int args, char* argc[]){
    pi=0;

    if(args < 1){
        printf("Excepted Arguments.\n");
        return -1;
    }

    n = atoi(argc[1]);
    
   
    thread_compute();

    printf("pi: %.10LF\n",pi);

    return 0;
    
}