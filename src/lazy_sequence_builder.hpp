#pragma once

#include "LAB2/isequencebuilder.hpp"
#include "LAB2/mutable_array_sequence.hpp"

template <typename T>
class LazySeqBuilder : public ISequenceBuilder<T> {
private:
    MutableArraySequence<T>* buffer;

public:
    LazySeqBuilder() : buffer(new MutableArraySequence<T>()) {}

    virtual ~LazySeqBuilder() {
        if (buffer) delete buffer;
    }

    void append(const T& item) override {
        buffer->append(item);
    }

    Sequence<T>* build() override {
        Sequence<T>* res = new LazySeq<T>(*buffer);
        delete buffer;
        buffer = nullptr;

        return res;
    }
};