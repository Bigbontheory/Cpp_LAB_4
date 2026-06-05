#pragma once 

#include <functional>
#include "i_generator.hpp"
#include <stdexcept>
#include <cstddef>

template<typename T>
class FunctionGenerator : public IGenerator<T> {
private:
	std::function<T(std::size_t)> func_;
	Ordinal length_;
	std::size_t current_index_ = 0;

public:
	FunctionGenerator(std::function<T(std::size_t)> func, Ordinal length) : func_(func), length_(length) {
		if (!func_) {
			throw std::invalid_argument("function cannot be null");
		}
	}

	T get_next() override {
		if (!has_next()) {
			throw std::out_of_range("Generator exhausted");
		}
		return func_(current_index_++);
	}

	bool has_next() const override {
		if (length_.is_infinite()) {
			return true;
		}
		return current_index_ < static_cast<std::size_t>(length_.get_value());
	}
	
	Ordinal length() const override {
		return length_;
	}

	FunctionGenerator<T>* clone() const override {
		return new FunctionGenerator<T>(*this);
	}
};