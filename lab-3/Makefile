SOURCE = mpi_trans.c
EXECS = mpi_trans
PROCS = 10
MATRIX = 100

MPICC = mpicc 
MPIRUN = mpirun

all = ${EXECS}

.PHONY: build run clean

build:
	@${MPICC} -o ${EXECS} ${SOURCE} -lm

run:
	@${MPIRUN} -np ${PROCS} ./${EXECS} ${MATRIX}

clean:
	@rm -rf ${all}