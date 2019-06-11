#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {


	MPI_Init(&argc, &argv);
	int rank, n, i, message;

	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 6) {

		std::cout << "Root process: " << rank << std::endl;

		for (i = 0; i < n; i++) {
			if (i != 6) {
				MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, 
						MPI_COMM_WORLD, &status);
				float div = message/static_cast<float>(rank);
				std::cout << "Hello from process " << message 
						<< " ~ 6 with div: " << div << std::endl;
			}
		}

	} else {

		MPI_Send(&rank, 1, MPI_INT, 6, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;

}