#pragma once
#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class AppendGenerator : public IGenerator<T> {
private:
    T item_;
    IGenerator<T>* base_gen_;
    bool item_yielded_;

public:
    AppendGenerator(const T& item, const IGenerator<T>* base_gen)
        : item_(item), base_gen_(nullptr), item_yielded_(false) {
        if (base_gen == nullptr) {
            throw std::invalid_argument("AppendGenerator: base_gen cannot be nullptr");
        }
        base_gen_ = base_gen->clone();
    }

    ~AppendGenerator() {
        delete base_gen_;
    }

    AppendGenerator(const AppendGenerator<T>& other)
        : item_(other.item_), base_gen_(nullptr), item_yielded_(other.item_yielded_) {
        if (other.base_gen_) {
            base_gen_ = other.base_gen_->clone();
        }
    }

    AppendGenerator<T>& operator=(const AppendGenerator<T>& other) {
        if (this != &other) {
            IGenerator<T>* new_base = other.base_gen_ ? other.base_gen_->clone() : nullptr;
            delete base_gen_;
            item_ = other.item_;
            base_gen_ = new_base;
            item_yielded_ = other.item_yielded_;
        }
        return *this;
    }

    bool has_next() const override {
        if (base_gen_->has_next()) {
            return true;
        }
        return !item_yielded_;
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("AppendGenerator: generator exhausted");
        }
        if (base_gen_->has_next()) {
            return base_gen_->get_next();
        }
        item_yielded_ = true;
        return item_;
    }

    IGenerator<T>* clone() const override {
        return new AppendGenerator<T>(*this);
    }

    Ordinal length() const override {
        return base_gen_->length() + Ordinal(0, 1);
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index < base_gen_->length()) {
            return base_gen_->get_by_ordinal(index);
        }
        if (index == base_gen_->length()) {
            return item_;
        }
        throw std::out_of_range("AppendGenerator: ordinal index out of range");
    }
};