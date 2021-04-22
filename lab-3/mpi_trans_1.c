#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
// #include <mpi.h>
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

int main(int argc, char *argv[]){
	// get raw by user
	n = atoi(argv[1]);
	int my_rank,group_size;
	int sqrt_group_size,u,v,length;//进程数开根号即每行或每列的子块数，列号，行号，每个块的维数 
	
	MPI_Init(&argc, &argv);

	// group_size is process num
	MPI_Comm_size(MPI_COMM_WORLD,&group_size);
	// my_rank is my PID
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);


	// count size of any matrix
	sqrt_group_size = sqrt(group_size);
	u = my_rank%sqrt_group_size;
	v = my_rank/sqrt_group_size;

	length = n/sqrt_group_size;
	tmp = (int *)malloc(sizeof(int)*length*length);
	if(my_rank == 0){
		init();
		getValue();
		// print();
   		t_start = MPI_Wtime();
		//这两层循环是找到对应编号的子块
		for(int i=0;i<sqrt_group_size;i++){
			for(int j=0;j<sqrt_group_size;j++){
				int point = 0;
				for(int k=length*i;k<length*(i+1);k++){
					for(int l=length*j;l<length*(j+1);l++){
						tmp[point] = matrix[k][l];
						point++;
					}
				}
				if(i==0 && j==0){
					for(int i=0;i<length;i++){
			            for(int j=0;j<length;j++){
			                out[i][j] = matrix[j][i];
			            }
			        }
				} 
				else{
					MPI_Send(tmp,length*length,MPI_INT,i*sqrt_group_size+j,i*sqrt_group_size+j,MPI_COMM_WORLD);
				}
			}
		}

    }
    else{
    	MPI_Recv(tmp,length*length,MPI_INT,0,my_rank,MPI_COMM_WORLD,&status);
    	int t;
        for(int i=0;i<length;i++){
            for(int j=i+1;j<length;j++){
                t = tmp[i*length+j];
                tmp[i*length+j] = tmp[j*length+i];
                tmp[j*length+i] = t;
            }
        }
        MPI_Send(tmp,length*length,MPI_INT,0,my_rank,MPI_COMM_WORLD);
    }
    
    if(my_rank==0){
        for(int i=0;i<sqrt_group_size;i++){

            for(int j=0;j<sqrt_group_size;j++){
            	if(i!=0 || j!=0){
	                MPI_Recv(tmp,length*length,MPI_INT,i*sqrt_group_size+j,i*sqrt_group_size+j,MPI_COMM_WORLD,&status);
					for(int x=0;x<length;x++){
	                    for(int y=0;y<length;y++){
	                        out[j*length+x][i*length+y] = tmp[x*length+y];
	                    }
	                }
            	}
            }
        }
        t_end = MPI_Wtime();
        printf("Matrix order:%d, Time cost:%lf\n",n,t_end-t_start);

        // print();

        free(matrix);
    }
    free(tmp);
	MPI_Finalize();
	return 0;
}

