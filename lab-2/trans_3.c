#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

int size, thread_nums;
int** matrix;
pthread_mutex_t lock;


void swap(int* a, int *b){
    *a=*a^*b;
    *b=*a^*b;
    *a=*b^*a;
}

void print_matrix(){

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}


void* trans(void* ID){

    int id = (int)ID;
    int piece = size/thread_nums;
    int end = (id == thread_nums-1) ? size : id + piece;
    int begin = id * piece;
    for(int rank=begin; rank<end; rank++){
    	for(int i=0; i<rank; i++){
            // printf("rank: %d, i: %d\n", rank, i);
	        swap(&matrix[rank][i], &matrix[i][rank]);
    	}
	}

}

int main(int args, char* argc[]){

    if(args < 2){
        printf("Expected Arguments");
        return -1;
    }

    size = atoi(argc[1]);
    thread_nums = atoi(argc[2]);

    int MAX_ROW , MAX_COL, side;
    side = MAX_ROW = MAX_COL = (int)sqrt((size*size)/thread_nums);
    
    matrix = (int**)malloc(size*sizeof(int*));
    for (int i = 0; i < size; i++){
       matrix[i] = (int*)malloc(size*sizeof(int));
    }

    // init matrix
    for (int i = 0; i < size; i++){
        for (int j = 0; j <size; j++){
            matrix[i][j] = i;
        }
    }

    // print init matrix
    print_matrix();


    pthread_t* threads  = (pthread_t*)malloc(sizeof(pthread_t*) * thread_nums);

    for (int i = 0; i < thread_nums; i++){
        pthread_create(&threads[i], NULL, trans, (void*)i);
    }

    for (int i = 0; i < thread_nums; i++){
        pthread_join(threads[i], NULL);
    }

    // print child matrix
    print_matrix();

}