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

	float recvbufprev, recvbufnext, sendbufprev, sendbufnext;

	MPI_Recv_init(&recvbufprev, 1, MPI_FLOAT, prev, 5, MPI_COMM_WORLD, &requests[0]);
	MPI_Recv_init(&recvbufnext, 1, MPI_FLOAT, next, 6, MPI_COMM_WORLD, &requests[1]);

	MPI_Send_init(&sendbufprev, 1, MPI_FLOAT, prev, 6, MPI_COMM_WORLD, &requests[2]);
	MPI_Send_init(&sendbufnext, 1, MPI_FLOAT, next, 5, MPI_COMM_WORLD, &requests[3]);

	sendbufprev = rank;
	sendbufnext = 10*rank;

	std::cout << "Current process: " << rank << ";\t";
	std::cout << "Sending to previous: " << sendbufprev << ";\t";
	std::cout << "Sending to next: " << sendbufnext << std::endl;

	MPI_Startall(4, requests);
	MPI_Waitall(4, requests, states);

	std::cout << "Current process: " << rank << ";\t";
	std::cout << "From previous got: " << recvbufprev << ";\t";
	std::cout << "From next got: " << recvbufnext << std::endl << std::endl;

	MPI_Request_free(&requests[0]);
	MPI_Request_free(&requests[1]);
	MPI_Request_free(&requests[2]);
	MPI_Request_free(&requests[3]);

	MPI_Finalize();

	return 0;
}