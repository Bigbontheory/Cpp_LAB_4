#pragma once

#include <stdexcept>
#include "i_transfinite_generator.hpp"
#include "ordinal.hpp"

template <class T>
class LazySeq;

template <class T>
class ConcatGenerator : public ITransfiniteGenerator<T> {
private:
    const LazySeq<T>& first_;
    const LazySeq<T>& second_;
    Ordinal len1_;
    Ordinal total_len_;
    std::size_t pos_;

public:
    ConcatGenerator(const LazySeq<T>& first, const LazySeq<T>& second)
        : first_(first),
        second_(second),
        len1_(first.get_ordinal_length()),
        total_len_(first.get_ordinal_length() + second.get_ordinal_length()), 
        pos_(0) {
    }

    Ordinal length() const override { return total_len_; }

    bool has_next() const override { // учесть, что последовательность может быть потенциально беск.
        if (total_len_.is_infinite()) return true;
        return pos_ < total_len_.get_finite_part();
    }

    T get_next() override {
        if (!has_next()) throw std::out_of_range("concat_generator: out of bounds");

        T val = get_by_ordinal_index(Ordinal(pos_));
        pos_++;
        return val;
    }

    T get_by_ordinal_index(const Ordinal& index) const override {
        if (index >= total_len_) {
            throw std::out_of_range("concat_generator: index out of bounds");
        }

        if (index < len1_) {
            return first_.get(index);
        }
        else {
 
            if (!len1_.is_infinite()) {             
                std::size_t relative_finite = index.get_finite_part() - len1_.get_finite_part();
                return second_.get(Ordinal(index.get_omega_count(), relative_finite));
            }
            else {
               
                std::size_t relative_omega = index.get_omega_count() - len1_.get_omega_count();
                std::size_t relative_finite = index.get_finite_part();

                return second_.get(Ordinal(relative_omega, relative_finite));
            }
        }
    }

    IGenerator<T>* clone() const override {
        return new ConcatGenerator<T>(first_, second_);
    }
};