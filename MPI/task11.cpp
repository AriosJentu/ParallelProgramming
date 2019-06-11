#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, size;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int msize = 500;
	int leftMatrix[msize][msize], 
		rightMatrix[msize][msize], 
		resultMatrix[msize][msize];

	for (int i = 0; i < msize; i++) {
		for (int j = 0; j < msize; j++) {
			leftMatrix[i][j] = 4;
			rightMatrix[i][j] = 2;
			resultMatrix[i][j] = 0;
		}
	}


	MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();

	for (int i = 0; i < msize; i++) {
		for (int j = 0; j < msize; j++) {
			for (int k = 0; k < msize; k++) {
				resultMatrix[i][j] += leftMatrix[k][j]*rightMatrix[i][k];
			}
		}
	}

	std::cout << "Process: " << rank << "; Finished at: " << MPI_Wtime() - start << std::endl;
	MPI_Finalize();

	return 0;
}
