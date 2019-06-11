#include <iostream>
#include <omp.h>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>

void client(omp_lock_t& read_lock, omp_lock_t& write_lock, std::string& input, const std::vector<std::string>& task) {
	for (int i = 0; i < task.size(); i++) {
		omp_set_lock(&write_lock);
		input = task[i];
		omp_unset_lock(&read_lock);
	}
}

void server(omp_lock_t& read_lock, omp_lock_t& write_lock, std::string& input) {

	int result = 0;
	int left, right;

	char oper;

	int tempi, tempc;

	while (true) {

		omp_set_lock(&read_lock);

		std::stringstream stream;
		stream << input;

		if (input.length() == 0) {
			std::cout << result << std::endl;
			omp_unset_lock(&write_lock);
			return;
		}

		if (stream.peek() == 'r') {
			stream.get();
			left = result;
			stream >> oper;
			if (stream.peek() == 'r') {
				right = result;
			}
		} else {
			stream >> left;
			stream >> oper;
			if (stream.peek() == 'r') {
				right = result;
			} else {
				stream >> right;
			}
		}

		switch (oper) {
			case '+':
				result = left + right;
			case '-':
				result = left - right;
			case '*':
				result = left * right;
			case '/':
				result = left / right;
		}

		omp_unset_lock(&write_lock);
	}
}

int main() {
	std::vector<std::string> task;
	task.push_back("5+3");
	task.push_back("res/2");
	task.push_back("24/res");
	task.push_back("res*res");
	task.push_back("res-6");
	task.push_back("res-2");
	task.push_back("");

	omp_lock_t read_lock, write_lock;
	
	omp_init_lock(&read_lock);
	omp_init_lock(&write_lock);

	omp_set_lock(&read_lock);

	std::string input;

	#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				server(read_lock, write_lock, input);
			}

			#pragma omp section
			{
				client(read_lock, write_lock, input, task);
			}
		}
	}
	return 0;
}