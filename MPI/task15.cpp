#include <mpi.h>
#include <iostream>

void printarrays(float* farr, char* carr, size_t size, int rank) {
	std::cout << "Rank: " << rank << std::endl;

	std::cout << "Array A: " << "[";
	for (int i = 0; i < size-1; i++) {
		std::cout << farr[i] << ", ";
	}
	std::cout << farr[size-1] << "]" << std::endl;

	std::cout << "Array B: " << "[";
	for (int i = 0; i < size-1; i++) {
		std::cout << carr[i] << ", ";
	}
	std::cout << carr[size-1] << "]" << std::endl;
}

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, size, nrank;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	float farr[10];
	char carr[10];
	char buffer[100];

	std::cout << "Initialize process ID: " << rank << std::endl;

	for (int i = 0; i < 10; i++) {
		farr[i] = rank + 1.0;
		carr[i] = 'a' + rank;
	}

	int pos = 0;

	if (rank > 0) {
		
		MPI_Bcast(buffer, 100, MPI_PACKED, 0, MPI_COMM_WORLD);

		pos = 0;
		MPI_Unpack(buffer, 100, &pos, farr, 10, MPI_FLOAT, MPI_COMM_WORLD);
		MPI_Unpack(buffer, 100, &pos, carr, 10, MPI_CHAR, MPI_COMM_WORLD);

		printarrays(farr, carr, 10, rank);
	
	} else {

		printarrays(farr, carr, 10, rank);

		MPI_Pack(farr, 10, MPI_FLOAT, buffer, 100, &pos, MPI_COMM_WORLD);
		MPI_Pack(carr, 10, MPI_CHAR, buffer, 100, &pos, MPI_COMM_WORLD);

		MPI_Bcast(buffer, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
	}

	std::cout << std::endl;

	MPI_Finalize();

	return 0;
}