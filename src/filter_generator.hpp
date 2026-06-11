#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class FilterGenerator : public IGenerator<T> {
private:
	IGenerator<T>* source_gen_;
	bool (*predicate_)(const T&);

public:
	FilterGenerator(IGenerator<T>* gen, bool (*pred)(const T&))
		: source_gen_(gen), predicate_(pred) {
	}

	T get_next() override {
		while (source_gen_->has_next()) {
			T item = source_gen_->get_next();
			if (predicate_(item)) {
				return item;
			}
		}
		throw std::out_of_range("No more elements matching the predicate");
	}

	bool has_next() const override {
		return source_gen_->has_next();
	}

	
	T get_by_ordinal(const Ordinal& index) const override {
		if (index.is_infinite()) {
			throw std::logic_error("IsInfinite not supported for get_by_ordinal in FilterGenerator");
		}

		int target_count = index.get_finite_part();
		int current_found = -1;

		while (source_gen_->has_next()) {
			T item = source_gen_->get_next();

			if (predicate_(item)) {
				current_found++;
				if (current_found == target_count) {
					return item;
				}
			}
		}

		throw std::out_of_range("Element not found: sequence ended before reaching the target index");
	}

	Ordinal length() const override {
		return source_gen_->length();
	}

	FilterGenerator<T>* clone() const override {
		return new FilterGenerator<T>(source_gen_, predicate_);
	}
};