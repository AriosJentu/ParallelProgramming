#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, size, nrank;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm comm;

	MPI_Comm_split(MPI_COMM_WORLD, rank%5, size-rank, &comm);
	MPI_Comm_rank(comm, &nrank);

	std::cout << "Rank: " << rank << "; Rank1: " << nrank << std::endl;

	MPI_Comm_free(&comm);
	MPI_Finalize();

	return 0; 
}