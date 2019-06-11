#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

#define MBSIZE 1024*1024
#define ARRLENGTH 4

void calc_capacity(int length, int rank) {

	char* message = new char[length];
	int count = 8;
	double finished_at = 0;
	double objsize = length/static_cast<double>(MBSIZE);

	for (int i = 0; i < count; i++) {
		if (rank == 0) {
			double start = MPI_Wtime();

			MPI_Send(message, length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(message, length, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			double finish = MPI_Wtime() - start;
			finished_at += finish;
			std::cout << "Finishing at: " << finish << "s for length " 
					<< objsize << "mb with iteration " << i << std::endl; 

		} else if (rank == 1) {

			MPI_Recv(message, length, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(message, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}
	}

	if (rank == 0) {
		std::cout << "::: Length: " << objsize << "; Finished at: " 
				<< finished_at/count << "s; Capacity is: " 
				<< 2*count*objsize/finished_at << "mbps" << std::endl << std::endl;
	}

	delete[] message;
}

void latency(int rank) {

	char message;
	int count = static_cast<int>(10e4);
	double finished_at = 0;

	for (int i = 0; i < count; i++) {

		if (rank == 0) {
			
			if (i%10000 == 0) {
				std::cout << "Fixed " << i << " iteration" << std::endl;
			}

			double start = MPI_Wtime();

			MPI_Send(&message, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&message, 0, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, 
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			finished_at += MPI_Wtime() - start;
		
		} else if (rank == 1) {
			MPI_Recv(&message, 0, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, 
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(&message, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}
	}

	if (rank == 0) {
		std::cout << "::: Latency: " << finished_at/(2*count) << "s" 
			<< std::endl << std::endl;
	}


}

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<int> elements(ARRLENGTH);
	for (int i = 0; i < ARRLENGTH; i++) {
		elements[i] = std::pow(2, ARRLENGTH+3-i);
	}
	
	for (int i = 0; i < ARRLENGTH; i++) {
		calc_capacity(MBSIZE*elements[i], rank);
	}

	latency(rank);

	MPI_Finalize();
	return 0;
}