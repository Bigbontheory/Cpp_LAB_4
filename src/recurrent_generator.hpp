#pragma once

#include "i_generator.hpp"
#include "LAB2/mutable_array_sequence.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class RecurrentGenerator : public IGenerator<T> {
private:
	T(*rule)(const MutableArraySequence<T>&);
	Ordinal length_;
	std::size_t current_index_;
	MutableArraySequence<T> window_;

public:
	RecurrentGenerator(const MutableArraySequence<T>& initial_sequence,
		T(*func)(const MutableArraySequence<T>&),
		Ordinal length) :
		rule(func),
		length_(length),
		current_index_(0),
		window_(initial_sequence) {
		if (func == nullptr) {
			throw std::invalid_argument("func cannot be nullptr");
		}
		if (initial_sequence.get_size() == 0) {
			throw std::invalid_argument("initial sequence cannot be empty");
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

		T next_val;

		if (current_index_ < window_.get_size()) {
			next_val = window_.get(current_index_);
		}

		else {
			next_val = rule(window_);
			window_.remove_at(0);
			window_.append(next_val);
		}
		current_index_++;
		return next_val;
	}

	RecurrentGenerator<T>* clone() const override {
		return new RecurrentGenerator<T>(*this);
	}

	T get_by_ordinal(const Ordinal& index) const override {
		if (!length_.is_infinite() && index >= length_) {
			throw std::out_of_range("RecurrentGenerator: index out of range");
		}
		if (index.is_infinite()) {
			throw std::out_of_range("RecurrentGenerator: infinite indices not supported");
		}
		std::size_t target = static_cast<std::size_t>(index.get_value());

		if (target < window_.get_size() && current_index_ <= target) {
			return window_.get(target);
		}
		RecurrentGenerator<T> temp = *this;
		T result;
		while (temp.current_index_ <= target) {
			result = temp.get_next();
		}
		return result;
	}

};