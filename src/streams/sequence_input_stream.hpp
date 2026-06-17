#pragma once

#include "i_input_stream.hpp"
#include "LAB2/sequence.hpp"
#include <stdexcept>

template <class T>
class SequenceInputStream : public IInputStream<T> {
private:
    const Sequence<T>* sequence_;
    int position_;
    bool is_open_;

public:
    explicit SequenceInputStream(const Sequence<T>* seq)
        : sequence_(seq), position_(0), is_open_(false) {
        if (sequence_ == nullptr) {
            throw std::invalid_argument("Sequence cannot be null");
        }
    }

    void open() override {
        is_open_ = true;
        position_ = 0;
    }

    void close() override {
        is_open_ = false;
    }

    bool is_end_of_stream() const override {
        if (!is_open_) return true;
        return position_ >= sequence_->get_size();
    }

    Ordinal get_position() const override {
        return Ordinal(0, position_);
    }

    bool is_can_seek() const override { return true; }
    bool is_can_go_back() const override { return true; }

    Ordinal seek(const Ordinal& index) override {
        if (!is_open_) {
            throw std::logic_error("Stream is closed");
        }

        int len = sequence_->get_size();
        int req_index = static_cast<int>(index.get_finite_part());

        if (req_index < 0) {
            position_ = 0;
        }
        else if (len != -1 && req_index > len) {
            position_ = len;
        }
        else {
            position_ = req_index;
        }

        return Ordinal(0, position_);
    }

    T input() override {
        if (!is_open_) {
            throw std::logic_error("Stream is closed");
        }
        if (is_end_of_stream()) {
            throw std::out_of_range("End of stream reached");
        }

        return (*sequence_)[position_++];
    }
};