#pragma once

#include "i_input_stream.hpp"
#include <stdexcept>

template <class T>
class LazyInputStream : public IInputStream<T> {
private:
    LazySeq<T>* sequence_;
    Ordinal position_;
    bool is_open_;

public:
    explicit LazyInputStream(LazySeq<T>* seq)
        : sequence_(seq), position_(Ordinal(0)), is_open_(false) {
    }

    virtual ~LazyInputStream() = default;

    void open() override {
        is_open_ = true;
        position_ = Ordinal(0);
    }

    void close() override {
        is_open_ = false;
    }

    bool is_end_of_stream() const override {
        if (!is_open_) return true;
        return position_ >= sequence_->get_ordinal_length();
    }

    Ordinal get_position() const override {
        return position_;
    }

    bool is_can_seek() const override { return true; }

    bool is_can_go_back() const override { return true; }

    Ordinal seek(const Ordinal& index) override {
        if (!is_open_) throw std::logic_error("Stream not open");

        Ordinal len = sequence_->get_ordinal_length();

        if (index > len) {
            position_ = len;
        }
        else {
            position_ = index;
        }

        return position_;
    }

    T input() override {
        if (!is_open_) throw std::logic_error("Stream not open");
        if (is_end_of_stream()) throw std::out_of_range("End of stream");

        T value = sequence_->get(position_);
        position_ = position_ + Ordinal(1);

        return value;
    }
};