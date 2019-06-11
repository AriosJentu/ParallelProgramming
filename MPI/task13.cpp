#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Group group, newgroup;
	MPI_Comm newcomm;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_group(MPI_COMM_WORLD, &group);

	int ranks[128], newrank;

	for (int i = 0; i < size/2; i++) {
		ranks[i] = i;
	}

	if (rank < size/2) {
		MPI_Group_incl(group, size/2, ranks, &newgroup);
	} else {
		MPI_Group_excl(group, size/2, ranks, &newgroup);
	}

	MPI_Comm_create(MPI_COMM_WORLD, newgroup, &newcomm);

	int buffer;
	MPI_Allreduce(&rank, &buffer, 1, MPI_INT, MPI_SUM, newcomm);
	MPI_Group_rank(newgroup, &newrank);

	std::cout << "Current rank is: " << rank << "; New rank is: " << newrank << "; Buffered: " << buffer << std::endl;

	MPI_Finalize();
	return 0;

}