#include <iostream>
#include <chrono>
#include <iomanip>

#include "ThreadPool.h"

using namespace std::chrono_literals;

int sum(int a, int b) {
	std::this_thread::sleep_for(3s);
	std::cout << "End sum a + b = " << a + b << std::endl;
	return a + b;
}

int sub(int a, int b) {
	std::this_thread::sleep_for(2s);
	std::cout << "End sum a - b = " << a - b << std::endl;
	return a - b;
}

int main() {
	int a = 10, b = 5, total_result = 0;
	std::cout << "first num = " << a << std::endl;
	std::cout << "second num = " << b << std::endl;

	ThreadPool tp;
	
	auto start = std::chrono::steady_clock::now();

	for (size_t i = 0; i < 3; i++) {
		tp.submit([&a, &b, &total_result] {
			total_result += sum(a, b);
			std::cout << "Total result = " << total_result << '\n' << std::string(20, '=') << std::endl;
		});
		tp.submit([&a, &b, &total_result] {
			total_result += sub(a, b);
			std::cout << "Total result = " << total_result << '\n' << std::string(20, '=') << std::endl;
			});
		std::this_thread::sleep_for(1s);
	}

	while (!tp.isEmpty()) {
		std::cout << "Number of Tasks in queue: " << tp.getSize() << std::endl;
		std::this_thread::sleep_for(500ms);
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "All time: " << std::setprecision(8) << std::fixed << elapsed_seconds.count() << "s\n";

	return 0;
}