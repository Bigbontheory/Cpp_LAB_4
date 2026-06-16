#include <gtest/gtest.h>
#include <stdexcept>
#include "sequence_generator.hpp"
#include "insert_at_generator.hpp"
#include "LAB2/mutable_array_sequence.hpp"
#include "ordinal.hpp"

TEST(InsertAtGeneratorTest, InsertInMiddle) {
    int raw_arr[] = { 10, 20, 30 };
    MutableArraySequence<int> source_seq(raw_arr, 3);
    SequenceGenerator<int> source_gen(source_seq);

    InsertAtGenerator<int> gen(&source_gen, 99, Ordinal(0, 1));

    EXPECT_EQ(gen.length(), Ordinal(0, 4));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 99);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 20);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 30);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(InsertAtGeneratorTest, InsertAtBeginning) {
    int raw_arr[] = { 10, 20 };
    MutableArraySequence<int> source_seq(raw_arr, 2);
    SequenceGenerator<int> source_gen(source_seq);

    InsertAtGenerator<int> gen(&source_gen, 99, Ordinal(0, 0));

    EXPECT_EQ(gen.length(), Ordinal(0, 3));
    EXPECT_EQ(gen.get_next(), 99);
    EXPECT_EQ(gen.get_next(), 10);
    EXPECT_EQ(gen.get_next(), 20);
}

TEST(InsertAtGeneratorTest, InsertAtEnd) {
    int raw_arr[] = { 10, 20 };
    MutableArraySequence<int> source_seq(raw_arr, 2);
    SequenceGenerator<int> source_gen(source_seq);

    InsertAtGenerator<int> gen(&source_gen, 99, Ordinal(0, 2));

    EXPECT_EQ(gen.length(), Ordinal(0, 3));
    EXPECT_EQ(gen.get_next(), 10);
    EXPECT_EQ(gen.get_next(), 20);
    EXPECT_EQ(gen.get_next(), 99);
}

TEST(InsertAtGeneratorTest, DirectAccessByOrdinal) {
    int raw_arr[] = { 11, 22, 33 };
    MutableArraySequence<int> source_seq(raw_arr, 3);
    SequenceGenerator<int> source_gen(source_seq);

    InsertAtGenerator<int> gen(&source_gen, 99, Ordinal(0, 1));

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0, 0)), 11);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0, 1)), 99);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0, 2)), 22);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0, 3)), 33);

    EXPECT_THROW(gen.get_by_ordinal(Ordinal(0, 4)), std::out_of_range);
}

TEST(InsertAtGeneratorTest, CloneIndependentState) {
    int raw_arr[] = { 10, 20 };
    MutableArraySequence<int> source_seq(raw_arr, 2);
    SequenceGenerator<int> source_gen(source_seq);

    InsertAtGenerator<int> gen(&source_gen, 99, Ordinal(0, 1));

    EXPECT_EQ(gen.get_next(), 10);

    IGenerator<int>* cloned_gen = gen.clone();

    EXPECT_TRUE(cloned_gen->has_next());
    EXPECT_EQ(cloned_gen->get_next(), 99);
    EXPECT_EQ(cloned_gen->get_next(), 20);
    EXPECT_FALSE(cloned_gen->has_next());

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 99);

    delete cloned_gen;
}

TEST(InsertAtGeneratorTest, ExceptionHandling) {
    int raw_arr[] = { 10, 20 };
    MutableArraySequence<int> source_seq(raw_arr, 2);
    SequenceGenerator<int> source_gen(source_seq);

    EXPECT_THROW(
        InsertAtGenerator<int> gen(&source_gen, 99, Ordinal(0, 3)),
        std::out_of_range
    );
}