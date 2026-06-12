#include <gtest/gtest.h>
#include <stdexcept>
#include "sequence_generator.hpp"
#include "LAB2/mutable_array_sequence.hpp"
#include "ordinal.hpp"

TEST(SequenceGeneratorTest, EmptySequence) {
    MutableArraySequence<int> empty_seq;
    SequenceGenerator<int> gen(empty_seq);

    EXPECT_EQ(gen.length(), Ordinal(0));
    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, StandardIteration) {
    int raw_arr[] = { 10, 20, 30 };
    MutableArraySequence<int> source_seq(raw_arr, 3);
    SequenceGenerator<int> gen(source_seq);

    EXPECT_EQ(gen.length(), Ordinal(3));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 20);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 30);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, CloneIndependentState) {
    int raw_arr[] = { 100, 200, 300 };
    MutableArraySequence<int> source_seq(raw_arr, 3);
    SequenceGenerator<int> gen(source_seq);

    EXPECT_EQ(gen.get_next(), 100);

    IGenerator<int>* cloned_gen = gen.clone();

    EXPECT_TRUE(cloned_gen->has_next());
    EXPECT_EQ(cloned_gen->get_next(), 200);
    EXPECT_EQ(cloned_gen->get_next(), 300);
    EXPECT_FALSE(cloned_gen->has_next());

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 200);

    delete cloned_gen;
}