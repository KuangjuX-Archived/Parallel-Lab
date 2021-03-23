#include <pthread.h>
#include <math.h>

int size, thread_nums;
int** matrix;

void* trans(void* ID){

}

int main(){
    size = thread_nums = 9;

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

    printf("\n");

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