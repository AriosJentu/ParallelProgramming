#include <mpi.h>
#include <iostream>
#include <vector>

void scalarprod(const std::vector<int> left, const std::vector<int> right, 
				int size, int rank, int count) {
	
	int chunksize = size / count;
	int lastsize = size - (count-1)*chunksize;

	int startindx = rank*chunksize;
	int nextchunkindx = startindx+chunksize;
	int localsum = 0;

	if (rank != count-1) {
		for (int i = startindx; i < nextchunkindx; i++) {
			localsum += left[i] * right[i];
		}
	} else {
		for (int i = startindx; i < startindx + lastsize; i++) {
			localsum += left[i] * right[i];
		}
	}

	int res = 0;

	MPI_Reduce(&localsum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (rank == 0) {
		std::cout << "Sum is: " << res << std::endl;
	}
}

int main(int argc, char* argv[]) {

	int vecsize = static_cast<int>(5e7);

	MPI_Init(&argc, &argv);
	int rank, n;

	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<int> left(vecsize, 2);
	std::vector<int> right(vecsize, 3);

	double start;
	if (rank == 0) {
		start = MPI_Wtime();
	}

	scalarprod(left, right, vecsize, rank, n);
	if (rank == 0) {
		std::cout << "Vector size is: " << vecsize << std::endl;
		std::cout << "Finished at: " << MPI_Wtime() - start << " ms." 
				<< std::endl;
	}

	MPI_Finalize();
	return 0;

}