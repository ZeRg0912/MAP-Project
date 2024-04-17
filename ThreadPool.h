#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "SafeQueue.h"

template<typename T>
class ThreadPool {
private:
	std::vector<std::thread> threads;
	SafeQueue<T> safe_queue;
	std::atomic_bool done;

public:
	ThreadPool() : done(false) {
		size_t const cores = std::thread::hardware_concurrency();
		for (size_t i = 1; i <= cores; i++) {
			threads.push_back(std::thread(&ThreadPool::work, std::move(this)));
		}
	};

	~ThreadPool() {
		done = true;
		for (auto& thread : threads) {
			thread.join();
		}
	};

	void work() {
		while (!done) {
			T task;
			safe_queue.pop(task);
		}
	};

	void submit(T task) {
		safe_queue.push(std::move(task));
	};

	void getTasks() {
		std::cout << "Tasks in queue: " << safe_queue.getTasks() << std::endl;
	};
};