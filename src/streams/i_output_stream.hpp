#pragma once

#include "ordinal.hpp"


template <class T>
class IOutputStream {
public:
    virtual ~IOutputStream() = default;

    virtual void open() = 0;
    virtual void close() = 0;

    virtual Ordinal get_position() const = 0;

    virtual Ordinal output(const T& item) = 0;
};