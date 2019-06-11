#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank1, rank2, size;

	MPI_Status state;
	MPI_Comm intercomm;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_spawn("task16slave", MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);

	int procs;
	MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &state);
	MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &state);
	MPI_Recv(&procs, 1, MPI_INT, 2, 2, intercomm, &state);

	std::cout << "Total processes count: " << procs << "; Slaves " << rank1 << " and " << rank2 << " are working" << std::endl;

	MPI_Finalize();
	return 0;
}