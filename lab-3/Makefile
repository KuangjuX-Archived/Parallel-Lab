SOURCE_1 = mpi_trans_1.c
SOURCE_2 = mpi_trans_2.c 

EXECS_1 = mpi_trans_1
EXECS_2 = mpi_trans_2

PROCS = 10
MATRIX = 1000

MPICC = mpicc 
MPIRUN = mpirun

all = ${EXECS_1} ${EXECS_2}

.PHONY: build_1 run_1 build_2 run_2 clean

build_1:
	@${MPICC} -o ${EXECS_1} ${SOURCE_1} -lm

run_1:
	@${MPIRUN} -np ${PROCS} ./${EXECS_1} ${MATRIX}

build_2:
	@${MPICC} -o ${EXECS_2} ${SOURCE_2} -lm

run_2:
	@${MPIRUN} -np ${PROCS} ./${EXECS_2} ${MATRIX}

clean:
	@rm -rf ${all}