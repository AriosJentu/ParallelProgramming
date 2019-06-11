#include <iostream>
#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>

void vmax(const std::vector<int> vec, int size) {

	int threadcnt = omp_get_max_threads();
	for (int threadn = 1; threadn <= threadcnt; threadn++) {

		omp_set_num_threads(threadn);

		double start = omp_get_wtime();

		int maxval = 0;
		#pragma omp parallel for
		for (int i = 0; i < size; i++) {
			maxval = std::max(maxval, vec[i]);
		}

		double end = omp_get_wtime();

		std::cout << " Result is " << maxval << ". Finished in " << end-start << " ms. Thread: " << threadn << std::endl; 
	}
}

int main(int argc, char* argv[]) {

	int size;
	sscanf(argv[1], "%i", &size);

	std::vector<int> vect(size);

	srand(time(0));
	
	for (int i = 0; i < size; i++) {
		int val = rand()%100;

		vect[i] = val;
		std::cout << val << " ";
	}

	std::cout << std::endl;

	vmax(vect, size);
	return 0;
}	