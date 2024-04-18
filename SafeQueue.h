#pragma once
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

template <typename T>
class SafeQueue {
private:
	std::queue<T> queue;
	mutable std::mutex mutex;
	std::condition_variable cond_var;

public:
	void push(T function) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(std::move(function));
		cond_var.notify_one();
	};

	T pop() {
		std::unique_lock<std::mutex> lock(mutex);
		cond_var.wait(lock, [this]() { return !queue.empty(); });
		T function = std::move(queue.front());
		queue.pop();
		return function;
	};

	size_t size() const {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.size();
	};

	bool empty() const {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.empty();
	}
};