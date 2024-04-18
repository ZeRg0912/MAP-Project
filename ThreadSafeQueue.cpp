#include <iostream>

#include "ThreadPool.h"

using namespace std::chrono_literals;

int sum(int a, int b) {
	std::this_thread::sleep_for(1s);
	return a + b;
}

int sub(int a, int b) {
	std::this_thread::sleep_for(2s);
	return a - b;
}

int main() {
	bool complete = false;
	int a = 10, b = 5;
	std::cout << "first num = " << a << std::endl;
	std::cout << "second num = " << b << std::endl;

	ThreadPool tp(1);

	for (size_t i = 0; i < 5; i++) {
		tp.submit([&a, &b] {
			sum(a, b);
		});
		tp.submit([&a, &b] {
			sub(a, b);
		});
		std::this_thread::sleep_for(1s);
	}
	return 0;
}