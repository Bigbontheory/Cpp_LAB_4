#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class ConcatGenerator : public IGenerator<T> {
private:
    IGenerator<T>* first_;
    IGenerator<T>* second_;
    Ordinal first_length_;
    Ordinal total_length_;
    std::size_t current_index_;

public:
    ConcatGenerator(IGenerator<T>* first, IGenerator<T>* second)
        : first_(first), second_(second), current_index_(0) {
        first_length_ = first_->length();
        total_length_ = first_length_ + second_->length();
    }

    ~ConcatGenerator()  {
        delete first_;
        delete second_;
    }

    bool has_next() const override {
        if (total_length_.is_infinite()) {
            return true;
        }
        return current_index_ < total_length_.get_value();
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("ConcatGenerator: Generator exhausted");
        }

        T value;
        if (first_->has_next()) {
            value = first_->get_next();
        }
        else {
            value = second_->get_next();
        }

        current_index_++;
        return value;
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index >= total_length_) {
            throw std::out_of_range("ConcatGenerator: index out of range");
        }

        if (index < first_length_) {
            return first_->get_by_ordinal(index);
        }
        else {
            return second_->get_by_ordinal(index - first_length_);
        }
    }

    Ordinal length() const override {
        return total_length_;
    }

    ConcatGenerator<T>* clone() const override {
        return new ConcatGenerator<T>(first_->clone(), second_->clone());
    }
};