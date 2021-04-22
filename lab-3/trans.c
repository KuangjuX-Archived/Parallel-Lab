#include <stdio.h>
#include <stdlib.h>

int size;
int **matrix;


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


int main(int args, char* argc[]){
    if (args < 1){
        printf("Expected Arguments.\n");
        return -1;
    }

    size = atoi(argc[1]);

    // alloc heap memory for matrix
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

    // print_matrix();

    for (int i = 0; i < size; i++){
        for(int j = i+1; j < size; j++){
            swap(&matrix[i][j], &matrix[j][i]);
        }
    }

    // print_matrix();

}