#include <iostream>
#include <chrono>

#include "ThreadPool.h"

using namespace std::chrono_literals;

int main() {
	Function<int> int_operation;
	Function<double> double_operation;

	int a = 10, b = 6;
	std::cout << "first num = " << a << std::endl;
	std::cout << "second num = " << b << std::endl;

	ThreadPool<int> tp;

	for (size_t i = 0; i < 3; i++) {
		tp.submit(int_operation.sum(a, b));
		tp.submit(int_operation.sub(a, b));
		std::this_thread::sleep_for(1s);
	}

	return 0;
}