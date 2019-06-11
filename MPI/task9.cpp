#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, size;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Request requests[4];
	MPI_Status states[4];

	int prev = (rank == 0) ? size-1 : rank-1;
	int next = (rank == size-1) ? 0 : rank+1;

	int bufprev, bufnext;

	MPI_Irecv(&bufprev, 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &requests[0]);
	MPI_Irecv(&bufnext, 1, MPI_INT, next, 6, MPI_COMM_WORLD, &requests[1]);

	MPI_Isend(&rank, 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &requests[2]);
	MPI_Isend(&rank, 1, MPI_INT, next, 5, MPI_COMM_WORLD, &requests[3]);

	MPI_Waitall(4, requests, states);

	std::cout << "Current process: " << rank << ";\t";
	std::cout << "From previous got: " << bufprev << ";\t";
	std::cout << "From next got: " << bufnext << std::endl;

	MPI_Finalize();

	return 0;
}