#pragma once

#include "LAB2/mutable_array_sequence.hpp"
#include "i_generator.hpp"
#include "LAB2/sequence.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class SequenceGenerator : public IGenerator<T> {
private:
    const MutableArraySequence<T> source_;
    std::size_t current_index_;

public:
    explicit SequenceGenerator(const MutableArraySequence<T>& source)
        : source_(source), current_index_(0) {
    }

    SequenceGenerator(const SequenceGenerator<T>& other)
        : source_(other.source_), current_index_(other.current_index_) {
    }

    SequenceGenerator<T>& operator=(const SequenceGenerator<T>& other) {
        if (this != &other) {
            source_ = other.source_;
            current_index_ = other.current_index_;
        }
        return *this;
    }

    ~SequenceGenerator() = default;

    bool has_next() const override {
        return current_index_ < source_.get_size();
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("SequenceGenerator: index out of range");
        }
        return source_.get(current_index_++);
    }

    IGenerator<T>* clone() const override {
        return new SequenceGenerator<T>(*this);
    }

    Ordinal length() const override {
        return Ordinal(0, source_.get_size());
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index.is_infinite()) {
            throw std::logic_error("SequenceGenerator: transfinite indexing is not supported for finite array sequences.");
        }

        std::size_t target_index = index.get_finite_part();
        if (target_index >= source_.get_size()) {
            throw std::out_of_range("SequenceGenerator: index out of range.");
        }
        return source_.get(target_index);
    }

};