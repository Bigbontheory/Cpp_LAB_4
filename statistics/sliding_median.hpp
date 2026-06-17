#pragma once
#include <stdexcept>
#include "sliding_cache.hpp"
#include "LAB2/mutable_array_sequence.hpp"

template<class T>
class SlidingMedian {
private:
    SlidingCache<T> window;

    void bubble_sort(MutableArraySequence<T>& array, int size) const {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (array.get(j) > array.get(j + 1)) {
                    T temp = array.get(j);
                    T next_val = array.get(j + 1);
                    array.remove_at(j + 1);
                    array.insert_at(temp, j + 1);

                    array.remove_at(j);
                    array.insert_at(next_val, j);
                }
            }
        }
    }

public:
    explicit SlidingMedian(int window_size) : window(window_size) {}

    void add(const T& value) {
        int logical_index = window.is_empty() ? 0 : window.get_last_index() + 1;
        window.push(value, logical_index);
    }

    double get_median() const {
        if (window.is_empty()) throw std::logic_error("Median of empty window");

        int size = window.get_count();
        MutableArraySequence<T> values;
        int logical_index = window.get_first_index();

        for (int i = 0; i < size; i++) {
            values.append(window.get(logical_index + i));
        }

        bubble_sort(values, size);

        if (size % 2 == 1) {
            return static_cast<double>(values.get(size / 2));
        }

        return (static_cast<double>(values.get(size / 2 - 1)) +
            static_cast<double>(values.get(size / 2))) / 2.0;
    }

    int get_window_size() const { return window.get_capacity(); }
    bool is_empty() const { return window.is_empty(); }
};