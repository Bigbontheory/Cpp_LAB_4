#pragma once

#include <stdexcept>

template<class T>
class OnlineStatistics {
private:
    T sum;
    T min_value;
    T max_value;
    int count;
    bool empty;

public:
    OnlineStatistics() : sum(0), count(0), empty(true) {}

    void add(const T& value) {
        if (empty) {
            min_value = value;
            max_value = value;
            empty = false;
        }
        else {
            if (value < min_value) min_value = value;
            if (value > max_value) max_value = value;
        }
        sum += value;
        count++;
    }

    double get_average() const {
        if (empty) throw std::logic_error("Statistics are empty");
        return static_cast<double>(sum) / count;
    }

    const T& get_min() const {
        if (empty) throw std::logic_error("Statistics are empty");
        return min_value;
    }

    const T& get_max() const {
        if (empty) throw std::logic_error("Statistics are empty");
        return max_value;
    }

    const T& get_sum() const { return sum; }
    int get_count() const { return count; }
    bool is_empty() const { return empty; }
};