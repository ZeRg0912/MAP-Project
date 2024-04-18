#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <string>

#include "SafeQueue.h"

static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

static void SetPosition(int x, int y)
{
	COORD point;
	point.X = x;
	point.Y = y;
	SetConsoleCursorPosition(hStdOut, point);
}

class ThreadPool {
private:
	std::vector<std::thread> threads;
	SafeQueue<std::function<void()>> safe_queue;
	bool done;
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	int cores;

public:
	ThreadPool(int _cores) : done(false), cores(_cores) {
		//size_t const cores = std::thread::hardware_concurrency();
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
			std::function<void()> task = safe_queue.pop();
			if (done && safe_queue.size() == 0) break;
			task();
		}
	};

	void submit(std::function<void()> function) {
		static int task_counter = 0;
		int task_id = ++task_counter;
		safe_queue.push([function, task_id, this]() {
			SetPosition(0, task_id + 1);
			std::string info;
			auto real_start = std::chrono::steady_clock::now();
			std::chrono::duration<double> real_function_duration = real_start - start_time;
			info += 
				"Task " 
				+ std::to_string(task_id) 
				+ " started: "
				+ std::to_string(real_function_duration.count());
			function();
			auto end_time = std::chrono::steady_clock::now();
			std::chrono::duration<double> duration = end_time - start_time;
			info +=
				" Task "
				+ std::to_string(task_id)
				+ " completed. Excecution time : "
				+ std::to_string(duration.count())
				+ "s"
				+ " Count of tasks: "
				+ std::to_string(getSize())
				+ "\n";
			std::cout << info;
			});
	};

	size_t getSize() const {
		return safe_queue.size();
	};

	bool isEmpty() const {
		return safe_queue.empty();
	}
};