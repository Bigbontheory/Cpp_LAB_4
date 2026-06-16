#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class PrependGenerator : public IGenerator<T> {
private:
    IGenerator<T>* source_gen_;
    T element_to_prepend_;

    Ordinal current_index_;
    Ordinal total_length_;

public:
    PrependGenerator(const IGenerator<T>* source, const T& element)
        : element_to_prepend_(element),
        current_index_(0, 0)
    {
        if (source == nullptr) {
            throw std::logic_error("PrependGenerator: source generator cannot be nullptr");
        }

        source_gen_ = source->clone();
        total_length_ = source_gen_->length() + Ordinal(0, 1);
    }

    PrependGenerator(const PrependGenerator& other)
        : source_gen_(other.source_gen_->clone()),
        element_to_prepend_(other.element_to_prepend_),
        current_index_(other.current_index_),
        total_length_(other.total_length_) {
    }

    ~PrependGenerator() {
        delete source_gen_;
    }

    Ordinal length() const override {
        return total_length_;
    }

    bool has_next() const override {
        return current_index_ < total_length_;
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("PrependGenerator: exhausted");
        }

        T val;

        if (current_index_ == Ordinal(0, 0)) {
            val = element_to_prepend_;
        }
        else {
            val = source_gen_->get_next();
        }

        current_index_ = current_index_ + Ordinal(0, 1);
        return val;
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index >= total_length_) {
            throw std::out_of_range("PrependGenerator: index out of range");
        }

        if (index == Ordinal(0, 0)) {
            return element_to_prepend_;
        }

        Ordinal relative_index = index;
        --relative_index;
        return source_gen_->get_by_ordinal(relative_index);
    }

    IGenerator<T>* clone() const override {
        return new PrependGenerator<T>(*this);
    }
};

