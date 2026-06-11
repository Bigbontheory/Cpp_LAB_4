#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>

template <typename T>
class MapGenerator : public IGenerator<T> {
private:
    IGenerator<T>* source_gen_;
    T(*mapper_)(const T&);

public:
    MapGenerator(IGenerator<T>* gen, T(*mapper)(const T&))
        : source_gen_(gen), mapper_(mapper) {
    }

    T get_next() override {
        return mapper_(source_gen_->get_next());
    }

    bool has_next() const override {
        return source_gen_->has_next();
    }

    Ordinal length() const override {
        return source_gen_->length();
    }

    T get_by_ordinal(const Ordinal& index) const override {
        return mapper_(source_gen_->get_by_ordinal(index));
    }

    MapGenerator<T>* clone() const override {
        return new  MapGenerator<T>(source_gen_, mapper_);
    }
};