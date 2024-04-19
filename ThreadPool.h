#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <iomanip>
#include <sstream>
#include <memory>

#include "SafeQueue.h"

static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std::chrono_literals;

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
	size_t const cores = std::thread::hardware_concurrency();
	std::mutex console_mutex;
	std::atomic<bool> done{ false };
	int task_counter;

	void printInfo(const int& position, const size_t& range, const std::string& info) {
		std::lock_guard<std::mutex> lock(console_mutex);
		SetPosition(static_cast<int>(range), position);
		std::cout << info;
	};

public:
	ThreadPool();

	~ThreadPool();

	void work();

	template<typename T>
	void submit(T(*function)(T a, T b, std::string result), T a, T b, std::string result_name) {
		safe_queue.push([function, a, b, result_name, this]() {
			int task_id = ++task_counter;
			std::string info;
			std::ostringstream oss;
			size_t length = 0;

			auto real_start = std::chrono::steady_clock::now();
			std::chrono::duration<double> real_function_duration = real_start - start_time;

			oss << std::fixed << std::setprecision(3) << real_function_duration.count();
			info =
				"Task "
				+ std::to_string(task_id)
				+ " started: "
				+ oss.str()
				+ "s"
				+ "  ";
			printInfo(task_id, length, info);

			length = info.size();
			T result = (*function)(a, b, result_name);

			oss.str("");
			oss << " a = " << a << ", b = " << b << "  " << result_name << " = " << result << '\t';
			info = oss.str();


			printInfo(task_id, length, info);

			length += info.size();

			auto end_time = std::chrono::steady_clock::now();
			std::chrono::duration<double> duration = end_time - start_time;

			oss.str("");
			oss << std::fixed << std::setprecision(3) << duration.count();
			info =
				"  Task "
				+ std::to_string(task_id)
				+ " completed. Duration: "
				+ oss.str()
				+ "s";

			printInfo(task_id, length, info);
		});
	};
};