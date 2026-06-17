#pragma once

#include "i_output_stream.hpp"
#include <stdexcept>

template <class T>
class LazyOutputStream : public IOutputStream<T> {
private:
    LazySeq<T>* sequence_;
    bool is_open_;

public:
    explicit LazyOutputStream(LazySeq<T>* seq)
        : sequence_(seq), is_open_(false) {
    }
    virtual ~LazyOutputStream() = default;

    void open() override {
        is_open_ = true;
    }

    void close() override {
        is_open_ = false;
    }

    Ordinal get_position() const override {
        return sequence_->get_ordinal_length();
    }

    Ordinal output(const T& item) override {
        if (!is_open_) throw std::logic_error("Stream not open");

        sequence_ = sequence_->append(item);

        return get_position();
    }

    LazySeq<T>* get_sequence() const {
        return sequence_;
    }
};