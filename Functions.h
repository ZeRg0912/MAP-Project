#pragma once
#include <iostream>
#include <functional>
#include <cmath>

template<typename T>
class Function {
public:
	std::function<T(T, T)> sum = [](const T& a, const T& b) { 
		std::cout << "first + second = " << a + b << std::endl;
		return a + b; 
		};
	std::function<T(T, T)> sub = [](const T& a, const T& b) {
		std::cout << "first - second = " << a + b << std::endl;
		return a - b; 
		};
	std::function<T(T, T)> mult = [](const T& a, const T& b) {
		std::cout << "first * second = " << a + b << std::endl;
		return a * b; 
		};
	std::function<T(T, T)> div = [](const T& a, const T& b) {
		std::cout << "first / second = " << a + b << std::endl;
		return a / b; 
		};
	std::function<T(T, T)> power = [](const T& num, const T& exp) {
		std::cout << "first^(second) = " << pow(num, exp) << std::endl;
		return pow(num, exp); 
		};
};