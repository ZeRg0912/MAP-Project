#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "Functions.h"

template <typename T>
class SafeQueue {
private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cond_var;

public:
	void push(T function) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(std::move(function));
		cond_var.notify_one();
	};

	bool pop(T& function) {
		std::lock_guard<std::mutex> lock(mutex);
		if (queue.empty()) return false;
		function = std::move(queue.front());
		queue.pop();
		return true;
	};

	bool empty() const {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.empty();
	};

	int getTasks() {
		return queue.size();
	};
};