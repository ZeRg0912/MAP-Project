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
	SafeQueue<Function<T>> safe_queue;
	std::atomic_bool done;

public:
	ThreadPool(){
		done = false;
		size_t const cores = std::thread::hardware_concurrency() - 1;
		for (size_t i = 0; i < cores; i++) {
			threads.push_back(std::thread(&ThreadPool::work, std::move(this)));
		}
	};

	~ThreadPool() {
		done = true;
		for (size_t i = 0; i < threads.size(); i++) {
			threads[i].join();
		}
	};

	void work() {
		
	};

	void submit(Function<T> task) {
		safe_queue.push(task);
	};
};