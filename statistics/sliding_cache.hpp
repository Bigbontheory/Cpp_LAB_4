#pragma once
#include "LAB2/mutable_array_sequence.hpp"

template <typename T>
class SlidingCache {
private:
    MutableArraySequence<T> cache_;
    int capacity_;
    int first_index_;
    int last_index_;

public:
    explicit SlidingCache(int window_size)
        : capacity_(window_size), first_index_(0), last_index_(-1) {
    }

    bool is_empty() const { return cache_.get_size() == 0; }
    int get_last_index() const { return last_index_; }
    int get_first_index() const { return first_index_; }
    int get_count() const { return cache_.get_size(); }
    int get_capacity() const { return capacity_; }

    void push(const T& value, int logical_index) {
        cache_.append(value);
        last_index_ = logical_index;

        if (cache_.get_size() > capacity_) {
            cache_.remove_at(0);
            first_index_++;
        }
    }

    T get(int logical_index) const {
        int physical_index = logical_index - first_index_;
        return cache_.get(physical_index);
    }
};