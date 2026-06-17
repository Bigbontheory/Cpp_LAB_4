#include <gtest/gtest.h>
#include <stdexcept>
#include "lazy_sequence.hpp"
#include "streams/lazy_output_streams.hpp"

TEST(LazyOutputStreamTest, UnopenedStreamThrows) {
    LazySeq<int> seq;
    LazyOutputStream<int> stream(&seq);

    EXPECT_THROW(stream.output(42), std::logic_error);
}

TEST(LazyOutputStreamTest, OpenAndCloseState) {
    LazySeq<int> seq;
    LazyOutputStream<int> stream(&seq);

    stream.open();
    EXPECT_NO_THROW(stream.output(10));

    stream.close();
    EXPECT_THROW(stream.output(20), std::logic_error);
}

TEST(LazyOutputStreamTest, OutputAdvancesPosition) {
    LazySeq<int> base_seq;
    LazyOutputStream<int> stream(&base_seq);
    stream.open();

    EXPECT_EQ(stream.get_position().get_finite_part(), 0);

    Ordinal pos1 = stream.output(100);
    EXPECT_EQ(pos1.get_finite_part(), 1);
    EXPECT_EQ(stream.get_position().get_finite_part(), 1);

    Ordinal pos2 = stream.output(200);
    EXPECT_EQ(pos2.get_finite_part(), 2);
    EXPECT_EQ(stream.get_position().get_finite_part(), 2);
}

TEST(LazyOutputStreamTest, FinalSequenceContainsCorrectData) {
    int arr[] = { 1, 2 };
    LazySeq<int> base_seq(arr, 2);
    LazyOutputStream<int> stream(&base_seq);
    stream.open();

    stream.output(3);
    stream.output(4);
    stream.close();

    LazySeq<int>* final_seq = stream.get_sequence();

    ASSERT_NE(final_seq, nullptr);
    EXPECT_EQ(final_seq->get_ordinal_length().get_finite_part(), 4);

    EXPECT_EQ(final_seq->get(0), 1);
    EXPECT_EQ(final_seq->get(1), 2);
    EXPECT_EQ(final_seq->get(2), 3);
    EXPECT_EQ(final_seq->get(3), 4);
}