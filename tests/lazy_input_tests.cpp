#include <gtest/gtest.h>
#include <stdexcept>
#include "lazy_sequence.hpp"
#include "streams/lazy_input_streams.hpp"

TEST(LazyInputStreamTest, UnopenedStreamThrows) {
    int arr[] = { 10, 20, 30 };
    LazySeq<int> seq(arr, 3);
    LazyInputStream<int> stream(&seq);

    EXPECT_TRUE(stream.is_end_of_stream());

    EXPECT_THROW(stream.input(), std::logic_error);
    EXPECT_THROW(stream.seek(Ordinal(1)), std::logic_error);
}

TEST(LazyInputStreamTest, OpenResetsPosition) {
    int arr[] = { 10, 20, 30 };
    LazySeq<int> seq(arr, 3);
    LazyInputStream<int> stream(&seq);

    stream.open();
    EXPECT_FALSE(stream.is_end_of_stream());
    EXPECT_EQ(stream.get_position().get_finite_part(), 0);
}

TEST(LazyInputStreamTest, SequentialRead) {
    int arr[] = { 10, 20, 30 };
    LazySeq<int> seq(arr, 3);
    LazyInputStream<int> stream(&seq);

    stream.open();

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.get_position().get_finite_part(), 1);

    EXPECT_EQ(stream.input(), 20);
    EXPECT_EQ(stream.get_position().get_finite_part(), 2);

    EXPECT_EQ(stream.input(), 30);
    EXPECT_EQ(stream.get_position().get_finite_part(), 3);
}

TEST(LazyInputStreamTest, EndOfStreamThrows) {
    int arr[] = { 42 };
    LazySeq<int> seq(arr, 1);
    LazyInputStream<int> stream(&seq);

    stream.open();
    EXPECT_FALSE(stream.is_end_of_stream());

    EXPECT_EQ(stream.input(), 42);

    EXPECT_TRUE(stream.is_end_of_stream());

    EXPECT_THROW(stream.input(), std::out_of_range);
}

TEST(LazyInputStreamTest, SeekToValidPosition) {
    int arr[] = { 10, 20, 30, 40, 50 };
    LazySeq<int> seq(arr, 5);
    LazyInputStream<int> stream(&seq);

    stream.open();

    stream.seek(Ordinal(3));
    EXPECT_EQ(stream.get_position().get_finite_part(), 3);
    EXPECT_EQ(stream.input(), 40);

    stream.seek(Ordinal(1));
    EXPECT_EQ(stream.get_position().get_finite_part(), 1);
    EXPECT_EQ(stream.input(), 20);
}

TEST(LazyInputStreamTest, SeekPastEndClamps) {
    int arr[] = { 1, 2, 3 };
    LazySeq<int> seq(arr, 3);
    LazyInputStream<int> stream(&seq);

    stream.open();

    stream.seek(Ordinal(100));

    EXPECT_EQ(stream.get_position().get_finite_part(), 3);
    EXPECT_TRUE(stream.is_end_of_stream());
}

TEST(LazyInputStreamTest, CloseStream) {
    int arr[] = { 10, 20, 30 };
    LazySeq<int> seq(arr, 3);
    LazyInputStream<int> stream(&seq);

    stream.open();
    EXPECT_EQ(stream.input(), 10);

    stream.close();

    EXPECT_TRUE(stream.is_end_of_stream());
    EXPECT_THROW(stream.input(), std::logic_error);
    EXPECT_THROW(stream.seek(Ordinal(0)), std::logic_error);
}

TEST(LazyInputStreamTest, StreamCapabilities) {
    LazySeq<int> seq;
    LazyInputStream<int> stream(&seq);

    EXPECT_TRUE(stream.is_can_seek());
    EXPECT_TRUE(stream.is_can_go_back());
}