#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>


int size, thread_nums;
int** matrix;
pthread_mutex_t lock;

void swap(int* a, int *b){
    *a = *a^*b;
    *b = *a^*b;
    *a = *b^*a;
}

void* child_trans(void* ID){
    int id = (int)ID;
    int side = (int)sqrt((size*size)/thread_nums);
    int row_block = size/side;

    int x_start = (id%row_block)*side;
    int x_end = x_start + side -1;
    int y_start = (id/row_block)*side;
    int y_end = y_start + side -1;

    pthread_mutex_lock(&lock);
    for(int i = 0; i < side; i++){
        for(int j = i; j < side; j++){
            swap(&matrix[i+x_start][j+y_start], &matrix[j+y_start][i+x_start]);
        }
    }
    pthread_mutex_unlock(&lock);

}

int main(){
    // cout<<"Please input size and thread_nums: \n";
    // cin>>size>>thread_nums;
    size = 4;
    thread_nums = 4;
    
     matrix = (int**)malloc(size*sizeof(int*));
    for (int i = 0; i < size; i++){
       matrix[i] = (int*)malloc(size*sizeof(int));
    }

    for (int i = 0; i < size; i++){
        for (int j = 0; j <size; j++){
            matrix[i][j] = i*j;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    
    pthread_t* threads  = (pthread_t*)malloc(sizeof(pthread_t*) * thread_nums);

    for (int i = 0; i < thread_nums; i++){
        pthread_create(&threads[i], NULL, child_trans, (void*)i);
    }

    for (int i = 0; i < thread_nums; i++){
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < size; i++){
        for (int j = 0; j <size; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

}