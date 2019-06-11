#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#define MAXSIZE 10

void printmat(int mtx[][MAXSIZE], int size) {
	std::cout << std::endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << mtx[i][j] << "\t"; 
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void multiplication(int left[][MAXSIZE], int right[][MAXSIZE], int size) {

	int res[size][MAXSIZE];

	int threadcnt = omp_get_max_threads();
	for (int threadn = 1; threadn <= threadcnt; threadn++) {

		double start = omp_get_wtime();

		#pragma omp parallel for num_threads(threadn) 
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				int sum = 0;
				for (int k = 0; k < size; k++) {
					sum += left[k][j]*right[i][k];
				}
				res[i][j] = sum;
			}
		}

		double end = omp_get_wtime();

		std::cout << "Finished in " << end-start << " ms. Thread: " << threadn << std::endl; 
		printmat(res, size);
	}

}

int main(int argc, char* argv[]) {

	int size;
	sscanf(argv[1], "%i", &size);

	int left[size][MAXSIZE];
	int right[size][MAXSIZE];

	srand(time(0));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			left[i][j] = rand()%100-50;
			right[i][j] = rand()%100-50;
		}
	}

	printmat(left, size);
	printmat(right, size);

	multiplication(left, right, size);

	return 0;
}