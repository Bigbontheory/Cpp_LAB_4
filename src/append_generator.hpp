#pragma once

#include <stdexcept>
#include "lazy_sequence.hpp"

template <class T>
class AppendGenerator : public ITransfiniteGenerator<T> {
private:
    const LazySeq<T>& source_;
    T* item_;
    Ordinal source_length_;
    std::size_t position_;

public:
    AppendGenerator(const LazySeq<T>& source, const T& item)
        : source_(source),
        item_(new T(item)),
        source_length_(source.get_ordinal_length()),
        position_(0) {
    }

    ~AppendGenerator() {
        delete item_;
    }

    Ordinal length() const override {
        return source_length_ + Ordinal(1);
    }

    bool has_next() const override {
        if (source_length_.is_infinite()) return true;
        return position_ <= source_length_.get_finite_part();
    }

    T get_next() override {
        if (!has_next()) throw std::out_of_range("append_generator: out of elements");

        T value;
        if (source_length_.is_infinite()) {
            value = source_.get(Ordinal(position_));
        }
        else {
            if (position_ < source_length_.get_finite_part()) {
                value = source_.get(Ordinal(position_));
            }
            else {
                value = *item_;
            }
        }

        position_++;
        return value;
    }

    IGenerator<T>* clone() const override {
        return new AppendGenerator<T>(source_, *item_);
    }

    T get_by_ordinal_index(const Ordinal& index) const override {
        if (index < source_length_) {
            return source_.get(index);
        }

        if (index == source_length_) {
            return *item_;
        }

        throw std::out_of_range("append_generator: transfinite index out of bounds");
    }
};