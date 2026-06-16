#pragma once

#include <stdexcept>
#include "i_generator.hpp"
#include "ordinal.hpp"

template <class T>
class LazySeq;

template <class T>
class MapGenerator : public IGenerator<T> {
private:
    const LazySeq<T>& source_;
    T(*func_)(const T&);;
    Ordinal source_length_;
    std::size_t position_;

public:
    MapGenerator(const LazySeq<T>& source, T(*func)(const T&))
        : source_(source),
        func_(func),
        source_length_(source.get_ordinal_length()),
        position_(0) {
    }

    Ordinal length() const override {
        return source_length_;
    }

    bool has_next() const override {
        if (source_length_.is_infinite()) return true;
        return position_ < source_length_.get_finite_part();
    }

    T get_next() override {
        if (!has_next()) throw std::out_of_range("map_generator: out of elements");

        T src_value = source_.get(Ordinal(position_));
        position_++;
        return func_(src_value);
    }

    IGenerator<T>* clone() const override {
        return new MapGenerator<T>(source_, func_);
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index >= source_length_) {
            throw std::out_of_range("map_generator: transfinite index out of bounds");
        }
        return func_(source_.get(index));
    }
};