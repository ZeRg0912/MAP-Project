#include "ThreadPool.h"

ThreadPool::ThreadPool() : task_counter(0) {
	for (size_t i = 0; i < cores; i++) {
		threads.emplace_back(&ThreadPool::work, this);
	}
};

ThreadPool::~ThreadPool() {
	done.store(true);
	for (auto& thread : threads) {
		thread.join();
	}
	SetPosition(0, task_counter + 1);
};

void ThreadPool::work() {
	while (!done.load() || !safe_queue.empty()) {
		std::function<void()> task;
		if (!safe_queue.empty()) {
			task = safe_queue.pop();
			task();
		}
	}
};