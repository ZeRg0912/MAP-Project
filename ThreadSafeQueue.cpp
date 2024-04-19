#include <iostream>
#include <random>

#include "ThreadPool.h"

template<typename T>
T Random(T min, T max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

using namespace std::chrono_literals;

int sum(int a, int b) {
	std::this_thread::sleep_for(1s);
	//std::cout << "a = " << a << ", b = " << b << " | a + b = " << a + b;
	return a + b;
}

int sub(int a, int b) {
	std::this_thread::sleep_for(2s);
	//std::cout << "a = " << a << ", b = " << b << " | a - b = " << a - b;
	return a - b;
}

int main() {
	bool complete = false;
	int min = 0, max = 10;

	int (*pt2Func)(int a, int b) = NULL;
	pt2Func = &sum;

	ThreadPool tp(4);
	std::cout << "Safe queue thread pool:\n";
	for (size_t i = 0; i < 20; i++) {
		tp.submit(pt2Func, Random(min, max), Random(min, max));
		tp.submit(pt2Func, Random(min, max), Random(min, max));
		std::this_thread::sleep_for(1s);
	}


	return 0;
}