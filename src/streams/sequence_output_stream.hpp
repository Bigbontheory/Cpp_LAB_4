#pragma once

#include <stdexcept>
#include "LAB2/sequence.hpp"
#include "i_output_stream.hpp"

template <class T>
class SequenceOutputStream : public IOutputStream<T> {
private:
    Sequence<T>* sequence_;
    int position_;
    bool is_open_;

public:
    explicit SequenceOutputStream(Sequence<T>* seq)
        : sequence_(seq), position_(0), is_open_(false) {
        if (sequence_ != nullptr) {
            position_ = sequence_->get_size();
        }
    }

    void open() override {
        is_open_ = true;
    }

    void close() override {
        is_open_ = false;
    }

    Ordinal get_position() const override {
        return Ordinal(0, position_);
    }

    Ordinal output(const T& item) override {
        if (!is_open_) {
            throw std::logic_error("Stream is closed");
        }
        if (sequence_->get_size() == -1) {
            throw std::runtime_error("Cannot output to an infinite sequence");
        }

        Sequence<T>* new_seq = sequence_->append(item);

        if (new_seq != sequence_) {
            delete sequence_;
        }

        sequence_ = new_seq;
        position_++;

        return Ordinal(0, position_);
    }

    Sequence<T>* get_sequence() const {
        return sequence_;
    }
};