#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class InsertElementGenerator : public IGenerator<T> {
private:
    IGenerator<T>* source_;
    T element_;
    const Ordinal  pos_;
    Ordinal current_index_;
    bool inserted_ = false;

public:
    InsertElementGenerator(IGenerator<T>* src, const T& el, Ordinal pos)
        : source_(src), element_(el), pos_(pos), current_index_(0) {
    }

    ~InsertElementGenerator() {
        delete source_;
    }

    T get_next() override {
        if (!inserted_ && current_index_ == pos_) {
            inserted_ = true;
            return element_;
        }

        T val = source_->get_next();
        current_index_ + Ordinal (0,1);
        return val;
    }

    bool has_next() const override {
        return source_->has_next() || (!inserted_ && current_index_==pos_);
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index == pos_) {
            return element_;
        }

        if (index < pos_) {
            return source_->get_by_ordinal(index);
        }
        else {
            return source_->get_by_ordinal(index - Ordinal(0, 1));
        }
    }

    Ordinal length() const override {

        return source_->length() + Ordinal(0,1);
    }

    InsertElementGenerator<T>* clone() const override {
        return new InsertElementGenerator<T>(source_->clone(), element_, pos_);
    }
};