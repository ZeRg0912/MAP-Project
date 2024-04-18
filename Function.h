#pragma once
#include <iostream>
#include <cmath>

template<typename T>
class Function {
public:
	std::function<T(T, T)> sum = [](const T& a, const T& b) {
		std::this_thread::sleep_for(1s);
		std::cout << "first + second = " << a + b << std::endl;
		return a + b; 
	};
	std::function<T(T, T)> sub = [](const T& a, const T& b) {
		std::this_thread::sleep_for(2s);
		std::cout << "first - second = " << a - b << std::endl;
		return a - b; 
	};
	std::function<T(T, T)> mult = [](const T& a, const T& b) {
		std::cout << "first * second = " << a * b << std::endl;
		std::this_thread::sleep_for(1000ms);
		return a * b; 
	};
	std::function<T(T, T)> div = [](const T& a, const T& b) {
		std::cout << "first / second = " << a / b << std::endl;
		std::this_thread::sleep_for(1000ms);
		return a / b; 
	};
	std::function<T(T, T)> power = [](const T& num, const T& exp) {
		std::cout << "first^(second) = " << pow(num, exp) << std::endl;
		std::this_thread::sleep_for(500ms);
		return pow(num, exp); 
	};

	std::function<void()> function(int function) {
		return [&T, &T]() { function(T, T); };
	}
};