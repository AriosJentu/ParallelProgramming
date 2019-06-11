#include <mpi.h>
#include <iostream>

#define MAXPROC 20

void timing(double start) {
	std::cout << "Finished in: " << MPI_Wtime() - start << " ms." << std::endl;
}

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, n, i, message;

	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double start = MPI_Wtime();

	if (rank == 0) {
		
		int curmax = MAXPROC;
		std::cout << "Root process: " << rank << std::endl;
		bool flag = false;

		for (i = 1; i < n; i++) {

			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, 
					MPI_COMM_WORLD, &status);
			
			std::cout << "Current message is: " << message 
					<< ";\tCurrent max is: " << curmax << std::endl;

			if (message >= curmax) {
				std::cout << "Not decreasing seq." << std::endl;
				timing(start);
				flag = true;
				break;
			} 

			curmax = message;
		}

		if (not flag) {
			std::cout << "Seq is decreasing" << std::endl;
			timing(start);
		}

	} else {
		
		MPI_Request req;
		MPI_Isend(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
	}

	MPI_Finalize();
	return 0;

}
