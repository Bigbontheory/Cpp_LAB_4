#pragma once

#include "ordinal.hpp"

template <class T>
class IInputStream {
public:
    virtual ~IInputStream() = default;
    virtual void open() = 0;
    virtual void close() = 0;

    virtual bool is_end_of_stream() const = 0;
    virtual Ordinal get_position() const = 0;

    virtual bool is_can_seek() const = 0;
    virtual bool is_can_go_back() const = 0;
    virtual Ordinal seek(const Ordinal& index) = 0;

    virtual T input() = 0;
};