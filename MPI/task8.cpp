#include <mpi.h>
#include <iostream>
#include <vector>

void mysum(int rank, int psize, const std::vector<int> &input) {
	int leftside = 2*rank+1;
	int rightside = 2*rank+2;

	if (rank == 0) {
	
		const int* position = input.data();
		int leftsize = input.size() / 2;
		int rightsize = input.size() - leftsize;

		double start = MPI_Wtime();

		MPI_Send(position, leftsize, MPI_INT, leftside, 0, MPI_COMM_WORLD);
		MPI_Send(position+leftsize, rightsize, MPI_INT, rightside, 0, MPI_COMM_WORLD);

		int leftsum, rightsum;
		MPI_Recv(&leftsum, 1, MPI_INT, leftside, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&rightsum, 1, MPI_INT, rightside, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		std::cout << "Default finished in: " << MPI_Wtime() - start << " with result " << leftsum+rightsum << std::endl;

		return;

	} else {

		MPI_Status state;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &state);

		int count;
		MPI_Get_count(&state, MPI_INT, &count);

		int* array = new int[count];

		MPI_Recv(array, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &state);

		int leftsize = count / 2;
		int rightsize = count - leftsize;

		int leftsum, rightsum;
		
		if (leftside < psize) {
		
			MPI_Send(array, leftsize, MPI_INT, leftside, 0, MPI_COMM_WORLD);
			MPI_Recv(&leftsum, 1, MPI_INT, leftside, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		} else {
			for (int i = 0; i < leftsize; i++) {
				leftsum += array[i];
			}
		}

		if (rightside < psize) {
		
			MPI_Send(array + leftsize, rightsize, MPI_INT, rightside, 0, MPI_COMM_WORLD);
			MPI_Recv(&rightsum, 1, MPI_INT, rightside, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		} else {
			for (int i = leftsize; i < count; i++) {
				rightsum += array[i];
			}
		}

		std::cout << leftsum << " " << rightsum << std::endl;
		int res = leftsum + rightsum;

		MPI_Send(&res, 1, MPI_INT, state.MPI_SOURCE, 0, MPI_COMM_WORLD);

		delete[] array;
	}
}

void sum(int rank, int psize, const std::vector<int> &input) {

	int rankingsize = (psize-1)*(input.size() / psize);
	int size = (rank == psize-1) ? input.size() - rankingsize : input.size() / psize;

	int res = 0;
	for (int i = 0; i < size; i++) {
		res += input[rank*(input.size() / psize) + i];
	}

	int result;
	double start = MPI_Wtime();

	MPI_Reduce(&res, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		std::cout << "Finished in: " << MPI_Wtime() - start << " with result " << result << std::endl;
	}
}

int main(int argc, char* argv[]) {

	int vecsize = static_cast<int>(5e7);

	MPI_Init(&argc, &argv);
	int rank, size;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<int> vec(vecsize, 1);

	mysum(rank, size, vec);
	sum(rank, size, vec);
	MPI_Finalize();

	return 0;
}