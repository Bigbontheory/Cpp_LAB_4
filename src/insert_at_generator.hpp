#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class InsertAtGenerator : public IGenerator<T> {
private:
    IGenerator<T>* source_gen_;
    T element_to_insert_;

    Ordinal insert_index_;
    Ordinal current_index_;
    Ordinal total_length_;
    bool element_inserted_;

public:
    InsertAtGenerator(const IGenerator<T>* source, const T& element, const Ordinal& insert_idx)
        : element_to_insert_(element),
        insert_index_(insert_idx),
        current_index_(0, 0),
        element_inserted_(false)
    {
        if (source == nullptr) {
            throw std::logic_error("InsertAtGenerator: source generator cannot be nullptr");
        }

        source_gen_ = source->clone();

        if (insert_idx > source_gen_->length()) {
            throw std::out_of_range("InsertAtGenerator: Insert index is greater than source length.");
        }

        total_length_ = source_gen_->length() + Ordinal(0, 1);
    }

    InsertAtGenerator(const InsertAtGenerator& other)
        : source_gen_(other.source_gen_->clone()),
        element_to_insert_(other.element_to_insert_),
        insert_index_(other.insert_index_),
        current_index_(other.current_index_),
        total_length_(other.total_length_),
        element_inserted_(other.element_inserted_) {
    }

    ~InsertAtGenerator() {
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
            throw std::out_of_range("InsertAtGenerator: exhausted");
        }

        T val;

        if (current_index_ < insert_index_) {
            val = source_gen_->get_next();
        }
        else if (current_index_ == insert_index_ && !element_inserted_) {
            val = element_to_insert_;
            element_inserted_ = true;
        }
        else {
            val = source_gen_->get_next();
        }

        current_index_ = current_index_ + Ordinal(0, 1);
        return val;
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index >= total_length_) {
            throw std::out_of_range("InsertAtGenerator: index out of range.");
        }
        if (index < insert_index_) {
            return source_gen_->get_by_ordinal(index);
        }
        if (index == insert_index_) {
            return element_to_insert_;
        }

        Ordinal relative_index = index;
        --relative_index;
        return source_gen_->get_by_ordinal(relative_index);
    }

    IGenerator<T>* clone() const override {
        return new InsertAtGenerator<T>(*this);
    }
};