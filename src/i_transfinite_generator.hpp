#pragma once
#include "i_generator.hpp"

template <typename T>
class ITransfiniteGenerator : public IGenerator<T> {
public:
    virtual T get_by_ordinal_index(const Ordinal& index) const = 0;
};