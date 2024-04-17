#include <iostream>
#include <chrono>
#include <iomanip>

#include "ThreadPool.h"

using namespace std::chrono_literals;

int main() {
	Function<int> int_operation;
	Function<double> double_operation;

	int a = 10, b = 5, c = 0;
	std::cout << "first num = " << a << std::endl;
	std::cout << "second num = " << b << std::endl;

	ThreadPool<int> tp;

	auto start = std::chrono::steady_clock::now();
	for (size_t i = 0; i < 3; i++) {
		std::cout << "c = " << c << std::endl;
		tp.submit(c += int_operation.sum(a, b));
		tp.submit(c += int_operation.sub(a, b));
		std::this_thread::sleep_for(1s);
		tp.getTasks();
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "All time: " << std::setprecision(8) << std::fixed << elapsed_seconds.count() << "s\n";

	return 0;
}