#pragma once
#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class PrependGenerator : public IGenerator<T> {
private:
    T item_;
    IGenerator<T>* base_gen_;
    bool item_yielded_;

public:
    PrependGenerator(const T& item, const IGenerator<T>* base_gen)
        : item_(item), base_gen_(nullptr), item_yielded_(false) {
        if (base_gen == nullptr) {
            throw std::invalid_argument("PrependGenerator: base_gen cannot be nullptr");
        }
        base_gen_ = base_gen->clone();
    }

    ~PrependGenerator() {
        delete base_gen_;
    }

    bool has_next() const override {
        if (!item_yielded_) {
            return true;
        }
        return base_gen_->has_next();
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("PrependGenerator: generator exhausted");
        }
        if (!item_yielded_) {
            item_yielded_ = true;
            return item_;
        }
        return base_gen_->get_next();
    }

    IGenerator<T>* clone() const override {
        return new PrependGenerator<T>(*this);
    }

    Ordinal length() const override {
        return Ordinal(0, 1) + base_gen_->length();
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index == Ordinal(0, 0)) {
            return item_;
        }
        if (index.is_infinite()) {
            throw std::out_of_range("PrependGenerator: infinite indices not supported");
        }
        return base_gen_->get_by_ordinal(index - Ordinal(0, 1));
    }
};