mpicc -o mpi_trans mpi_trans.c -lm && \
mpirun -np 5 ./mpi_trans 100