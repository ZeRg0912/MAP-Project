#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <iomanip>
#include <sstream>

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
	SafeQueue<std::function<void()>> safe_queue;
	std::vector<std::thread> threads;
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	//size_t const cores = std::thread::hardware_concurrency();
	size_t const cores;
	std::mutex console_mutex;
	bool done;
	int task_counter;

public:
	ThreadPool(size_t _cores) : done(false), cores(_cores), task_counter(0){
		for (size_t i = 0; i < cores; i++) {
			threads.emplace_back(&ThreadPool::work, this);
		}
	};

	~ThreadPool() {
		done = true;
		for (auto& thread : threads) {
			thread.join();
		}
		SetPosition(0, task_counter + 1);
	};

	void work() {
		while (!done || !safe_queue.empty()) {
			std::function<void()> task;
			if (!safe_queue.empty()) {
				task = safe_queue.pop();
				task();
			}
			else {
				std::this_thread::yield();
			}
		}
	};

	void submit(int (*function)(int a, int b), int a, int b) {
		safe_queue.push([function, a, b, this]() {
			int task_id = ++task_counter;
			std::string info;
			std::ostringstream oss;

			auto real_start = std::chrono::steady_clock::now();
			std::chrono::duration<double> real_function_duration = real_start - start_time;

			oss << std::fixed << std::setprecision(3) << real_function_duration.count();
			info =
				"Task "
				+ std::to_string(task_id)
				+ " started: "
				+ oss.str()
				+ "s"
				+ "\t";
			SetPosition(0, task_id);
			std::cout << info;
			int length = info.size();

			int result = (*function)(a, b);

			oss.str("");
			oss << " a = " << a << ", b = " << b << " Result = " << result << '\t';
			info = oss.str();
			SetPosition(length, task_id);
			std::cout << info;
			length += info.size();

			auto end_time = std::chrono::steady_clock::now();
			std::chrono::duration<double> duration = end_time - start_time; 

			oss.str("");
			oss << std::fixed << std::setprecision(3) << duration.count();
			info =
				"Task "
				+ std::to_string(task_id)
				+ " completed. Duration: "
				+ oss.str()
				+ "s"
				+ ". Count of tasks: "
				+ std::to_string(getSize());
			SetPosition(length, task_id);
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