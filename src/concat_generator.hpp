#pragma once

#include <stdexcept>
#include <cstddef>
#include "i_generator.hpp"
#include "ordinal.hpp"

template <class T>
class ConcatGenerator : public IGenerator<T> {
private:
    IGenerator<T>* first_;
    IGenerator<T>* second_;
    Ordinal len1_;
    Ordinal total_len_;
    mutable bool first_exhausted_;

public:
    ConcatGenerator(IGenerator<T>* first, IGenerator<T>* second)
        : first_(first ? first->clone() : nullptr),
        second_(second ? second->clone() : nullptr),
        len1_(first ? first->length() : Ordinal(0)),
        total_len_((first ? first->length() : Ordinal(0)) + (second ? second->length() : Ordinal(0))),
        first_exhausted_(false) {

        if (!first_ || !second_) {
            throw std::invalid_argument("ConcatGenerator: operands cannot be nullptr");
        }
    }

    ConcatGenerator(const ConcatGenerator<T>& other)
        : first_(other.first_->clone()),
        second_(other.second_->clone()),
        len1_(other.len1_),
        total_len_(other.total_len_),
        first_exhausted_(other.first_exhausted_) {
    }

    ConcatGenerator<T>& operator=(const ConcatGenerator<T>& other) {
        if (this != &other) {
            delete first_;
            delete second_;
            first_ = other.first_->clone();
            second_ = other.second_->clone();
            len1_ = other.len1_;
            total_len_ = other.total_len_;
            first_exhausted_ = other.first_exhausted_;
        }
        return *this;
    }

    ~ConcatGenerator() override {
        delete first_;
        delete second_;
    }

    Ordinal length() const override {
        return total_len_;
    }

    bool has_next() const override {
        if (first_exhausted_) {
            return second_->has_next();
        }

        if (first_->has_next()) {
            return true;
        }

        first_exhausted_ = true;
        return second_->has_next();
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("ConcatGenerator: no more elements");
        }

        if (!first_exhausted_) {
            return first_->get_next();
        }

        return second_->get_next();
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index >= total_len_) {
            throw std::out_of_range("ConcatGenerator: index out of bounds");
        }

        if (index < len1_) {
            return first_->get_by_ordinal(index);
        }
        else {
            if (!len1_.is_infinite()) {
                if (index.is_infinite()) {
                    return second_->get_by_ordinal(index);
                }
                else {
                    std::size_t relative_finite = index.get_finite_part() - len1_.get_finite_part();
                    return second_->get_by_ordinal(Ordinal(0, relative_finite));
                }
            }
            else {
                if (index.get_omega_count() == len1_.get_omega_count()) {
                    std::size_t relative_finite = index.get_finite_part() - len1_.get_finite_part();
                    return second_->get_by_ordinal(Ordinal(0, relative_finite));
                }
                else {
                    std::size_t relative_omega = index.get_omega_count() - len1_.get_omega_count();
                    std::size_t relative_finite = index.get_finite_part();
                    return second_->get_by_ordinal(Ordinal(relative_omega, relative_finite));
                }
            }
        }
    }

    IGenerator<T>* clone() const override {
        return new ConcatGenerator<T>(*this);
    }
};