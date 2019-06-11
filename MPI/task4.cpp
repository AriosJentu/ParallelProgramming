#include <mpi.h>
#include <iostream>

#define SIZE 100

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	int rank, n, i;
	int len, buf;
	char *name = new char;

	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name, &len);

	double start = MPI_Wtime();
	double finish;

	for (i = 0; i < SIZE; i++) {
		finish = MPI_Wtime();
	}

	if (rank != 0) {
		MPI_Recv(&buf, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, 
				MPI_STATUS_IGNORE);
	}

	std::cout << "Processor: " << name << "; Process: " << rank 
			<< "; Timing: " << (finish-start)/SIZE << std::endl;

	if (rank != n-1) {
		MPI_Send(&buf, 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}