#pragma once
#include <stdexcept>
#include "online_statistics.hpp"
#include "sliding_median.hpp"


template <typename T>
class IInputStream;

template<class T>
class StatisticsProcessor {
private:
    IInputStream<T>* stream;
    OnlineStatistics<T> statistics;
    SlidingMedian<T> median;

public:
    StatisticsProcessor(IInputStream<T>* input_stream, int median_window_size)
        : stream(input_stream), median(median_window_size) {
    }

    void process_next() {
        if (stream == nullptr) throw std::invalid_argument("Stream is nullptr");
        if (stream->is_end_of_stream()) throw std::out_of_range("End of stream");

        T value = stream->input();

        statistics.add(value);
        median.add(value);
    }

    double get_average() const { return statistics.get_average(); }
    double get_median() const { return median.get_median(); }
    const T& get_min() const { return statistics.get_min(); }
    const T& get_max() const { return statistics.get_max(); }
    const T& get_sum() const { return statistics.get_sum(); }
    int get_count() const { return statistics.get_count(); }
    bool is_empty() const { return statistics.is_empty(); }
};