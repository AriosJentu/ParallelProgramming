#include <iostream>
#include <omp.h>
#include <string>
#include <sstream>
#include <cstdlib>

int main() {

	std::stringstream arr[8];
	int count;
	#pragma omp parallel
	{
		count = omp_get_num_threads(); 
		int num = omp_get_thread_num();
		arr[num] << "Поток номер " << num << std::endl;
	}

	for (int i = count-1; i != 0; i--) {
		std::cout << arr[i].str();
	}

	return 0;
}