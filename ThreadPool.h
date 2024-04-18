#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "SafeQueue.h"

class ThreadPool {
private:
	std::vector<std::thread> threads;
	SafeQueue<std::function<void()>> safe_queue;
	bool done;

public:
	ThreadPool() : done(false) {
		size_t const cores = std::thread::hardware_concurrency();
		for (size_t i = 0; i < cores; i++) {
			threads.emplace_back(&ThreadPool::work, this);
		}
	};

	~ThreadPool() {
		done = true;
		for (auto& thread : threads) {
			thread.join();
		}
	};

	void work() {
		while (true) {
			std::cout << "Start task!\n";
			std::function<void()> task = safe_queue.pop();
			if (done && safe_queue.size() == 0) break;
			task();
		}
	};

	void submit(std::function<void()> function) {
		safe_queue.push(std::move(function));
	};

	size_t getSize() {
		return safe_queue.size();
	};

	bool isEmpty() {
		return safe_queue.empty();
	}
};