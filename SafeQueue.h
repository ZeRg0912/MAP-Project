#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "Functions.h"

template <typename T>
class SafeQueue {
private:
	std::queue<Function<T>> queue;
	std::mutex mutex;
	std::condition_variable cond_var;

public:
	void push(Function<T> function) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(std::move(Function<T>(function)));
		cond_var.notify_one();
	};

	T pop() {
		std::lock_guard<std::mutex> lock(mutex);
		cond_var.wait(lock, [this] { return !queue.empty(); });
		auto function = queue.front();
		queue.pop();
		return function;
	};

	bool empty() const {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.empty();
	};
};