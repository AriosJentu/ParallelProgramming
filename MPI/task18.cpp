#include <mpi.h>
#include <iostream>
#include <fstream>

#define BSIZE 100

void createfile() {
	std::ofstream file;
	file.open("file.txt");
	
	for (int i = 0; i < BSIZE*10; i++) {
		file << 'a';
	}

	file.close();
}

int main(int argc, char* argv[]) {

	createfile();
	MPI_Init(&argc, &argv);
	
	MPI_Status state;
	MPI_File file;

	char buffer[BSIZE];

	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
	MPI_File_set_view(file, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);

	int sum = 0;
	int count;
	
	do {

		MPI_File_read(file, buffer, BSIZE, MPI_CHAR, &state);
		MPI_Get_count(&state, MPI_CHAR, &count);
		std::cout << "Buffer: " << buffer << std::endl;

		sum += count;

	} while (count >= BSIZE);

	MPI_File_close(&file);

	std::cout << "Sum is " << sum << std::endl;
	MPI_Finalize();

	remove("file.txt");
	return 0;
}