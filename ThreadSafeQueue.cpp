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

int sum(int a, int b, std::string name) {
	std::this_thread::sleep_for(5s);
	return a + b;
}

int sub(int a, int b, std::string name) {
	std::this_thread::sleep_for(4s);
	return a - b;
}

int main() {

	bool complete = false;
	int min = 0, max = 10;

	int (*sumFunc)(int a, int b, std::string name) = &sum;
	int (*subFunc)(int a, int b, std::string name) = &sub;

	ThreadPool tp;
	std::cout << "Safe queue thread pool:\n";
	for (size_t i = 0; i < 10; i++) {
		tp.submit(sumFunc, Random(min, max), Random(min, max), "sum");
		tp.submit(subFunc, Random(min, max), Random(min, max), "sub");
		std::this_thread::sleep_for(1s);
	}


	return 0;
}