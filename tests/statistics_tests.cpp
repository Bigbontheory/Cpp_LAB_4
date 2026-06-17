
#include <gtest/gtest.h>
#include <stdexcept>


#include "../statistics/online_statistics.hpp"
#include "../statistics/sliding_cache.hpp"
#include "../statistics/sliding_median.hpp"
#include "../statistics/statistics_processor.hpp"


#include "streams/i_input_stream.hpp" 
#include "ordinal.hpp"

template <typename T>
class StubInputStream : public IInputStream<T> {
private:
    T* data_;
    int size_;
    int current_index_;
    bool is_open_;

public:
    StubInputStream(T* array, int size)
        : data_(array), size_(size), current_index_(0), is_open_(false) {
    }

    void open() override {
        is_open_ = true;
    }

    void close() override {
        is_open_ = false;
    }

    bool is_end_of_stream() const override {
        return current_index_ >= size_;
    }

    Ordinal get_position() const override {
        return Ordinal(current_index_);
    }

    bool is_can_seek() const override {
        return false;
    }

    bool is_can_go_back() const override {
        return false;
    }

    Ordinal seek(const Ordinal& index) override {
        throw std::logic_error("Seek is not supported in StubInputStream");
    }

    T input() override {
        if (!is_open_) throw std::runtime_error("Stream is closed");
        if (is_end_of_stream()) throw std::out_of_range("End of stream");
        return data_[current_index_++];
    }
};


TEST(OnlineStatisticsTest, EmptyStatisticsThrowsExceptions) {
    OnlineStatistics<int> stats;

    EXPECT_TRUE(stats.is_empty());
    EXPECT_EQ(stats.get_count(), 0);
    EXPECT_EQ(stats.get_sum(), 0);
    EXPECT_THROW(stats.get_min(), std::logic_error);
    EXPECT_THROW(stats.get_max(), std::logic_error);
    EXPECT_THROW(stats.get_average(), std::logic_error);
}

TEST(OnlineStatisticsTest, AccumulatesCorrectValues) {
    OnlineStatistics<int> stats;

    stats.add(10);
    stats.add(20);
    stats.add(5);

    EXPECT_FALSE(stats.is_empty());
    EXPECT_EQ(stats.get_count(), 3);
    EXPECT_EQ(stats.get_sum(), 35);
    EXPECT_EQ(stats.get_min(), 5);
    EXPECT_EQ(stats.get_max(), 20);
    EXPECT_DOUBLE_EQ(stats.get_average(), 35.0 / 3.0);
}


TEST(SlidingCacheTest, MaintainedCapacityAndShiftingIndices) {
    SlidingCache<int> cache(3);

    EXPECT_TRUE(cache.is_empty());
    EXPECT_EQ(cache.get_capacity(), 3);

    cache.push(10, 0);
    cache.push(20, 1);
    cache.push(30, 2);

    EXPECT_EQ(cache.get_count(), 3);
    EXPECT_EQ(cache.get_first_index(), 0);
    EXPECT_EQ(cache.get_last_index(), 2);
    EXPECT_EQ(cache.get(0), 10);
    EXPECT_EQ(cache.get(2), 30);

    cache.push(40, 3);

    EXPECT_EQ(cache.get_count(), 3);
    EXPECT_EQ(cache.get_first_index(), 1);
    EXPECT_EQ(cache.get_last_index(), 3);

    EXPECT_EQ(cache.get(1), 20);
    EXPECT_EQ(cache.get(3), 40);

    EXPECT_THROW(cache.get(0), std::out_of_range);
}


TEST(SlidingMedianTest, ThrowsOnEmptyWindow) {
    SlidingMedian<int> median(3);
    EXPECT_THROW(median.get_median(), std::logic_error);
}

TEST(SlidingMedianTest, CalculatesMedianCorrectly) {
    SlidingMedian<double> median(3);


    median.add(10.0);
    EXPECT_DOUBLE_EQ(median.get_median(), 10.0);

    median.add(20.0);
    EXPECT_DOUBLE_EQ(median.get_median(), 15.0);

    median.add(5.0);
    EXPECT_DOUBLE_EQ(median.get_median(), 10.0);

    median.add(15.0);
    EXPECT_DOUBLE_EQ(median.get_median(), 15.0);
}


TEST(StatisticsProcessorTest, FullStreamProcessingIntegrativeTest) {
    const int size = 5;
    int raw_data[size] = { 1, 10, 2, 8, 5 };

    StubInputStream<int> stream(raw_data, size);
    stream.open();

    StatisticsProcessor<int> processor(&stream, 3);

    EXPECT_TRUE(processor.is_empty());

    processor.process_next();
    EXPECT_EQ(processor.get_count(), 1);
    EXPECT_EQ(processor.get_sum(), 1);
    EXPECT_DOUBLE_EQ(processor.get_median(), 1.0);

    processor.process_next();
    EXPECT_EQ(processor.get_min(), 1);
    EXPECT_EQ(processor.get_max(), 10);
    EXPECT_DOUBLE_EQ(processor.get_average(), 5.5);
    EXPECT_DOUBLE_EQ(processor.get_median(), 5.5);

    processor.process_next();
    EXPECT_DOUBLE_EQ(processor.get_median(), 2.0);

    processor.process_next();
    EXPECT_EQ(processor.get_count(), 4);
    EXPECT_EQ(processor.get_sum(), 21);
    EXPECT_EQ(processor.get_min(), 1);
    EXPECT_DOUBLE_EQ(processor.get_median(), 8.0);

    processor.process_next();
    EXPECT_DOUBLE_EQ(processor.get_median(), 5.0);

    EXPECT_TRUE(stream.is_end_of_stream());
    EXPECT_THROW(processor.process_next(), std::out_of_range);

    stream.close();
}
