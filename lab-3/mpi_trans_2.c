#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

int **matrix, **out, *tmp;
int n;
double t_start, t_end;
MPI_Status status;

void init(){
	matrix = (int **)malloc(n * sizeof(int *));
	out = (int **)malloc(n * sizeof(int *));
    for(int i=0;i<n;i++){
        matrix[i] = (int *)malloc(n * sizeof(int));
        out[i] = (int *)malloc(n * sizeof(int));
    }
}

void getValue(){
	int cnt = 0;
    for(int i=0;i<n;i++){
    	for(int j=0;j<n;j++){
    		matrix[i][j] = out[i][j] = cnt;
    		cnt++;
		}
	}
}


void print(){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%d ",out[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


int main(int argc, char* argv[]) {
    n = atoi(argv[1]);

    int my_rank, group_size;

    // piece is length of every child matrix
    int piece;

    MPI_Init(&argc, &argv);

    // group_size is process num
	MPI_Comm_size(MPI_COMM_WORLD,&group_size);
	// my_rank is my PID
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    piece = n/group_size;
	tmp = (int*)malloc(sizeof(int)*n*n);


	if(my_rank == 0) {
		init();
		getValue();
		// print();
		t_start = MPI_Wtime();

		for (int i=0; i<group_size; i++) {

			int begin = i * piece;
			int end = (i == group_size-1) ? group_size : begin + piece;
			int point = 0;
			int rec_size = 0;
			
			for(int rank = begin; rank < end; rank++) {
				rec_size += 2*(rank+1)-1;
			}

			// printf("rec size: %d\n", rec_size);
			// tmp = (int*)malloc(sizeof(int)*rec_size);

			for(int rank = begin; rank < end; rank++) {
				for (int j=0; j<rank; j++) {
					tmp[point] = matrix[rank][j];
					tmp[rec_size-point-1] = matrix[j][rank];
					point++;
					// printf("matrix[%d][%d] = %d\n", rank, j, matrix[rank][j]);
					// printf("matrix[%d][%d] = %d\n", j, rank, matrix[j][rank]);
				}
			}

			// for(int i=0; i<rec_size; i++) {
			// 	printf("tmp[%d]: %d\n", i, tmp[i]);
			// }



			if(i == 0){
				for(int row=0; row<piece; row++) {
					for(int col=0; col<piece; col++) {
						out[row][col] = matrix[col][row];
					}
				}
			}else {
				MPI_Send(tmp, rec_size, MPI_INT, i, i, MPI_COMM_WORLD);
			}
		}
	}else {
		int rec_size = 0;
		int begin = my_rank * piece;
		int end = (my_rank == group_size-1) ? group_size : begin + piece;
		for(int rank = begin; rank < end; rank++) {
				rec_size += 2*(rank+1)-1;
		}
		MPI_Recv(tmp, rec_size, MPI_INT, 0, my_rank, MPI_COMM_WORLD, &status);

		// for(int i=0; i<rec_size; i++) {
		// 	printf("Before tmp[%d]: %d\n", i, tmp[i]);
		// }

		for (int i = begin; i < end; i++) {
			for(int j = 0; j < i; j++) {
				int x = (i-begin)*(end-begin)+j;
				int temp = tmp[x];
				tmp[x] = tmp[rec_size-x-1];
				tmp[rec_size-x-1] = temp;
			}
		}

		// for(int i=0; i<rec_size; i++) {
		// 	printf("After tmp[%d]: %d\n", i, tmp[i]);
		// }

		MPI_Send(tmp, rec_size, MPI_INT, 0, my_rank, MPI_COMM_WORLD);

	}

	if (my_rank == 0) {

		for(int i=0; i<group_size; i++) {
			if(i != 0) {
				int begin = i * piece;
				int end = (i == group_size-1) ? group_size : begin + piece;
				int rec_size = 0;
				
				for(int rank = begin; rank < end; rank++) {
					rec_size += 2*(rank+1)-1;
				}
				MPI_Recv(tmp, rec_size, MPI_INT, i, i, MPI_COMM_WORLD, &status);

				int point = 0;
				for(int rank = begin; rank < end; rank++) {
					for (int j=0; j<rank; j++) {
						out[rank][j] = tmp[point];
						out[j][rank] = tmp[rec_size-point-1];
						point++;
					}
				}
			}
		}
		t_end = MPI_Wtime();
		printf("Matrix order:%d, Time cost:%lf\n",n,t_end-t_start);


        free(matrix);
	}

	free(tmp);
	MPI_Finalize();
	return 0;
}