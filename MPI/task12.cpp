#include <mpi.h>
#include <iostream>

void maxval(void* a, void* b, int* l, MPI_Datatype* type) {
	for (int i = 0; i < *l; i++) {
		((int*)b)[i] = std::max( ((int*)a)[i], ((int*)b)[i] );
	}
}

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, size;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Op maxoper;
	MPI_Op_create(&maxval, 1, &maxoper);

	int a = rank;
	int reduce, orig;

	MPI_Reduce(&a, &reduce, 1, MPI_INT, maxoper, 0, MPI_COMM_WORLD);
	MPI_Reduce(&a, &orig, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	MPI_Op_free(&maxoper);

	if (rank == 0) {
		std::cout << "Result of maxval is: " << reduce << std::endl;
		std::cout << "Result of MPI_MAX is: " << orig << std::endl;
	}

	MPI_Finalize();
	return 0;

}