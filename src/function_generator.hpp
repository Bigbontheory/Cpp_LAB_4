#pragma once 

#include "i_generator.hpp"
#include <stdexcept>
#include <cstddef>
#include "ordinal.hpp"

template <typename T> 
class FunctionGenerator : public IGenerator<T> {
private:
	T(*func_)(std::size_t);
	Ordinal length_;
	std::size_t current_index_;

public:
	FunctionGenerator(T(*func)(std::size_t), Ordinal length)
		: func_(func), length_(length), current_index_(0) {
		if (func == nullptr) {
			throw std::invalid_argument("func cannot be nullptr");
		}
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

	T get_next() override {
		if (!has_next()) {
			throw std::out_of_range("Generator exhausted");
		}
		return func_(current_index_++); 
	}

	FunctionGenerator<T>* clone() const override {
		return new FunctionGenerator<T>(*this);
	}

	T get_by_ordinal(const Ordinal& index) const override {
		if (length_.is_infinite() || index.is_infinite()) {
			throw std::logic_error("FunctionGenerator: random access is not supported if the sequence or index is infinite.");
		}
		std::size_t target_index = index.get_finite_part();
		if (target_index >= static_cast<std::size_t>(length_.get_value())) {
			throw std::out_of_range("FunctionGenerator: index out of range.");
		}
		return func_(target_index);
	}
};


