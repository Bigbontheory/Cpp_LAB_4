#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class ReplaceAtGenerator : public IGenerator<T> {
private:
    IGenerator<T>* source_gen_;
    T new_element_;
    Ordinal target_index_;
    Ordinal current_index_;

public:
    ReplaceAtGenerator(const IGenerator<T>* source, const T& element, const Ordinal& target_idx)
        : new_element_(element),
        target_index_(target_idx),
        current_index_(0, 0)
    {
        if (source == nullptr) {
            throw std::logic_error("ReplaceAtGenerator: source generator cannot be nullptr");
        }

        source_gen_ = source->clone();

        if (target_idx >= source_gen_->length()) {
            throw std::out_of_range("ReplaceAtGenerator: Target index is out of bounds of source length.");
        }
    }

    ReplaceAtGenerator(const ReplaceAtGenerator& other)
        : source_gen_(other.source_gen_->clone()),
        new_element_(other.new_element_),
        target_index_(other.target_index_),
        current_index_(other.current_index_) {
    }

    ReplaceAtGenerator& operator=(const ReplaceAtGenerator& other) {
        if (this != &other) {
            delete source_gen_;
            source_gen_ = other.source_gen_->clone();
            new_element_ = other.new_element_;
            target_index_ = other.target_index_;
            current_index_ = other.current_index_;
        }
        return *this;
    }

    ~ReplaceAtGenerator() override {
        delete source_gen_;
    }

    Ordinal length() const override {
        return source_gen_->length();
    }

    bool has_next() const override {
        return source_gen_->has_next();
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("ReplaceAtGenerator: exhausted");
        }

        T val = source_gen_->get_next();
        if (current_index_ == target_index_) {
            val = new_element_;
        }

        current_index_ = current_index_ + Ordinal(0, 1);
        return val;
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index >= source_gen_->length()) {
            throw std::out_of_range("ReplaceAtGenerator: index out of range.");
        }

        if (index == target_index_) {
            return new_element_;
        }

        return source_gen_->get_by_ordinal(index);
    }

    IGenerator<T>* clone() const override {
        return new ReplaceAtGenerator<T>(*this);
    }
};