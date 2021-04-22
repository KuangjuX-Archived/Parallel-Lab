make build_1 && \
make build_2 && \
make trans && \
qsub mpi_trans_1.pbs && \
qsub mpi_trans_2.pbs && \
qsub trans.pbs