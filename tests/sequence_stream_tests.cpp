#include <gtest/gtest.h>
#include <stdexcept>
#include "ordinal.hpp"
#include "LAB2/mutable_array_sequence.hpp"
#include "streams/sequence_input_stream.hpp"
#include "streams/sequence_output_stream.hpp"

// ---------------------------------------------------------
// Тесты Потока Ввода (SequenceInputStream)
// ---------------------------------------------------------
TEST(SequenceStreamsTest, InputStream_Navigation) {
    MutableArraySequence<int> seq;
    seq.append(10);
    seq.append(20);
    seq.append(30);

    SequenceInputStream<int> stream(&seq);

    stream.open();

    EXPECT_FALSE(stream.is_end_of_stream());
    // Сравниваем finite_part, так как operator== может отсутствовать
    EXPECT_EQ(stream.get_position().get_finite_part(), 0);

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.get_position().get_finite_part(), 1);

    EXPECT_EQ(stream.input(), 20);
    EXPECT_EQ(stream.get_position().get_finite_part(), 2);

    stream.seek(Ordinal(0, 0));
    EXPECT_EQ(stream.input(), 10);

    stream.seek(Ordinal(0, 2));
    EXPECT_EQ(stream.input(), 30);

    EXPECT_TRUE(stream.is_end_of_stream());
    EXPECT_THROW(stream.input(), std::out_of_range);

    stream.close();
}

TEST(SequenceStreamsTest, InputStream_ClosedThrows) {
    MutableArraySequence<int> seq;
    seq.append(100);

    SequenceInputStream<int> stream(&seq);

    EXPECT_THROW(stream.input(), std::logic_error);
    EXPECT_THROW(stream.seek(Ordinal(0, 0)), std::logic_error);

    stream.open();
    stream.close();

    EXPECT_THROW(stream.input(), std::logic_error);
    EXPECT_THROW(stream.seek(Ordinal(0, 0)), std::logic_error);
}

TEST(SequenceStreamsTest, OutputStream_Mutation) {
    Sequence<int>* seq = new MutableArraySequence<int>();
    SequenceOutputStream<int> stream(seq);

    stream.open();

    EXPECT_EQ(stream.get_position().get_finite_part(), 0);

    Ordinal pos1 = stream.output(777);
    EXPECT_EQ(pos1.get_finite_part(), 1);

    Ordinal pos2 = stream.output(888);
    EXPECT_EQ(pos2.get_finite_part(), 2);

    stream.close();

    Sequence<int>* final_seq = stream.get_sequence();
    EXPECT_EQ(final_seq->get_size(), 2);
    EXPECT_EQ((*final_seq)[0], 777);
    EXPECT_EQ((*final_seq)[1], 888);

    delete final_seq;
}

TEST(SequenceStreamsTest, OutputStream_ClosedThrows) {
    Sequence<int>* seq = new MutableArraySequence<int>();
    SequenceOutputStream<int> stream(seq);

    EXPECT_THROW(stream.output(555), std::logic_error);

    stream.open();
    stream.close();

    EXPECT_THROW(stream.output(999), std::logic_error);

    delete stream.get_sequence();
}