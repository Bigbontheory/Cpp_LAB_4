#include <gtest/gtest.h>
#include <stdexcept>
#include "sequence_generator.hpp"
#include "prepend_generator.hpp"
#include "lazy_sequence.hpp"
#include "LAB2/mutable_array_sequence.hpp"
#include "ordinal.hpp"

TEST(PrependGeneratorTest, BasicIteration) {
    int raw_arr[] = { 10, 20 };
    MutableArraySequence<int> source_seq(raw_arr, 2);
    SequenceGenerator<int> source_gen(source_seq);

    PrependGenerator<int> gen(&source_gen, 99);

    EXPECT_EQ(gen.length(), Ordinal(0, 3));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 99);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 20);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(PrependGeneratorTest, DirectAccessByOrdinal) {
    int raw_arr[] = { 10, 20 };
    MutableArraySequence<int> source_seq(raw_arr, 2);
    SequenceGenerator<int> source_gen(source_seq);

    PrependGenerator<int> gen(&source_gen, 99);

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0, 0)), 99);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0, 1)), 10);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0, 2)), 20);

    EXPECT_THROW(gen.get_by_ordinal(Ordinal(0, 3)), std::out_of_range);
}

TEST(PrependGeneratorTest, CloneIndependentState) {
    int raw_arr[] = { 10, 20 };
    MutableArraySequence<int> source_seq(raw_arr, 2);
    SequenceGenerator<int> source_gen(source_seq);

    PrependGenerator<int> gen(&source_gen, 99);

    EXPECT_EQ(gen.get_next(), 99);

    IGenerator<int>* cloned_gen = gen.clone();

    EXPECT_TRUE(cloned_gen->has_next());
    EXPECT_EQ(cloned_gen->get_next(), 10);
    EXPECT_EQ(cloned_gen->get_next(), 20);
    EXPECT_FALSE(cloned_gen->has_next());

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);

    delete cloned_gen;
}

TEST(PrependGeneratorTest, NullptrExceptionHandling) {
    EXPECT_THROW(
        PrependGenerator<int> gen(nullptr, 99),
        std::logic_error
    );
}
