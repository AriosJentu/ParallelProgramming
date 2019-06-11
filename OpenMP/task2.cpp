#include <iostream>
#include <omp.h>
#include <vector>
#include <string>
#include <sstream>

void sum(int size) {

	std::vector<int> sumarray(size, 2);
	int sum = 0;

	#pragma omp for
	for (int i = 0; i < size; i++) {
		sum += sumarray[i];
	}

	std::cout << ":::RC::: " << size << " elements array's sum is " << sum << std::endl;


	//Atomic expr
	sum = 0;
	#pragma omp parallel 
	{
		int localsum = 0;
		
		#pragma omp for
		for (int i = 0; i < size; i++) {
			localsum += sumarray[i];
		}

		#pragma omp atomic
		sum += localsum;
	}

	std::cout << ":::AE::: " << size << " elements array's sum is " << sum << std::endl;


	//Parallel Reduction
	sum = 0;
	#pragma omp parallel for reduction(+:sum)
	for (int i = 0; i < size; i++) {
		sum += sumarray[i];
	}

	std::cout << ":::PR::: " << size << " elements array's sum is " << sum << std::endl;


	//Critical Section
	sum = 0;
	#pragma omp parallel
	{
		int localsum = 0;
		
		#pragma omp for
		for (int i = 0; i < size; i++) {
			localsum += sumarray[i];
		}

		#pragma omp critical
		{
			sum += localsum;
		}
	}

	std::cout << ":::CS::: " << size << " elements array's sum is " << sum << std::endl;

}

int main() {

	int cntreduct = 0;
	#pragma omp parallel reduction(+:cntreduct)
	{
		cntreduct++;
	}
	std::cout << "Serial area count is " << cntreduct << std::endl;

	int arr1[10], arr2[10], arr3[10];
	int i, cur;
	for (i = 0; i < 10; i++) {
		arr1[i] = i;
		arr2[i] = 2*i;
		arr3[i] = 0;
	}	

	std::stringstream ssarr[10];
	#pragma omp parallel shared(arr1, arr2, arr3) private(i, cur)
	{
		cur = omp_get_thread_num();

		#pragma omp for
		for (i = 0; i < 10; i++) {
			arr3[i] = arr1[i] + arr2[i];
			ssarr[i] << "Thread " << cur << " uses index " << i << std::endl;
			std::cout << ssarr[i].str();
		}
	}

	sum(1e6);
	return 0;
}