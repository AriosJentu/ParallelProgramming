#include <iostream>
#include <cstdio>
#include <omp.h>
#include <vector>

void scalarproduct(const std::vector<int> left, const std::vector<int> right, int size) {

	int threadcnt = omp_get_max_threads();
	for (int threadn = 1; threadn <= threadcnt; threadn++) {

		long production = 0;
		double start = omp_get_wtime();

		#pragma omp parallel num_threads(threadcnt)
		{

			long localprod = 0;
			#pragma omp for
			for (int i = 0; i < size; i++) {
				localprod += left[i] * right[i];
			}

			#pragma omp atomic
			production += localprod;

		}

		double end = omp_get_wtime();

		std::cout << "Result is " << production << ". Finished in " << end-start << " ms. Thread: " << threadn << std::endl; 
	}
}

int main(int argc, char* argv[]) {

	int size, init;
	sscanf(argv[1], "%i", &size);
	sscanf(argv[2], "%i", &init);

	std::vector<int> left(size, init);
	std::vector<int> right(size, init);

	scalarproduct(left, right, size);
	return 0;

}