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
	std::this_thread::sleep_for(2s);
	return a + b;
}

int sub(int a, int b) {
	std::this_thread::sleep_for(1s);
	return a - b;
}

int main() {
	bool complete = false;
	int min = 0, max = 10;

	int (*sumFunc)(int a, int b) = &sum;
	int (*subFunc)(int a, int b) = &sub;

	ThreadPool tp(2);
	std::cout << "Safe queue thread pool:\n";
	for (size_t i = 0; i < 5; i++) {
		tp.submit(sumFunc, Random(min, max), Random(min, max));
		tp.submit(subFunc, Random(min, max), Random(min, max));
		std::this_thread::sleep_for(1s);
	}


	return 0;
}