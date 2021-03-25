#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>


int size, thread_nums;
int** matrix;
pthread_mutex_t lock;

struct params{
    int id;
    int side;
};

void swap(int* a, int *b){
    *a=*a^*b;
    *b=*a^*b;
    *a=*b^*a;
}


void print_matrix(){

    for (int i = 0; i < size; i++){
        for (int j = 0; j <size; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}


void* trans(void* args){
    struct params *recv;
    recv = (struct params*)args;
    int id = recv->id;
    int side = recv->side;
    int row_block = size/side;
    int x = id % row_block;
    int y = id / row_block;
    int x_start = x*side;
    int y_start = y*side;
    int sym_id = x * side + y;
    int sym_x_start = (sym_id%row_block)*side;
    int sym_y_start = (sym_id/row_block)*side;
    for (int i = 0; i < side; i++){
        for (int j = 0; j < side; j++){
            // debug for swap
            // printf("swap: (%d, %d),  (%d, %d)\n", i+x_start, j+y_start, sym_x_start+i, sym_y_start+j);
            swap(&matrix[i+x_start][j+y_start], &matrix[sym_x_start+i][sym_y_start+j]);
        }
    }
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
        for(int j = i + 1; j < side; j++){
            // debug for swap 
            // printf("swap: (%d, %d),  (%d, %d)\n", i+x_start, j+y_start, j+x_start, i+y_start);
            swap(&matrix[i+x_start][j+y_start], &matrix[j+x_start][i+y_start]);
        }
    }
    pthread_mutex_unlock(&lock);

}


int main(int argc, char* argv[]){
    if (argc < 1){
        printf("Expected Arguments");
        return -1;
    }

    int m = atoi(argv[1]);
    size = m*m;
    thread_nums = size;
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
    // print_matrix();

    pthread_t* threads  = (pthread_t*)malloc(sizeof(pthread_t*) * thread_nums);

    for (int i = 0; i < thread_nums; i++){
        pthread_create(&threads[i], NULL, child_trans, (void*)i);
    }

    for (int i = 0; i < thread_nums; i++){
        pthread_join(threads[i], NULL);
    }

    free(threads);

    // print child matrix
    // print_matrix();
    int nums = m*(m-1)/2;
    pthread_t* threads_2 = (pthread_t*)malloc(sizeof(pthread_t*) * nums);
    // local transpose
    int count = 0;
    for(int row = 0; row<MAX_ROW; row++){
        for(int col = row+1; col<MAX_ROW; col++){
            int id = row*MAX_ROW + col;
            struct params arg;
            arg.id = id;
            arg.side = side;
            pthread_create(&threads_2[count], NULL, trans, &arg);
            count++;
        }
    }

    for (int i = 0; i < nums; i++){
        pthread_join(threads_2[i], NULL);
    }

    // print matrix trans result
    // print_matrix();
    return 0;

}