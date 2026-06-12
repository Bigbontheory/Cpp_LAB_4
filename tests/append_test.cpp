#include <gtest/gtest.h>
#include "append_generator.hpp"
#include "lazy_sequence.hpp"

TEST(AppendGeneratorTest, FiniteSequenceBehavior) {
    int arr[] = { 10, 20 };
    LazySeq<int> source(arr, 2);

    AppendGenerator<int> gen(source, 30);

    EXPECT_EQ(gen.length(), Ordinal(3));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);
    EXPECT_EQ(gen.get_next(), 20);
    EXPECT_EQ(gen.get_next(), 30);
    EXPECT_FALSE(gen.has_next());


    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(0)), 10);
    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(1)), 20);
    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(2)), 30);

    EXPECT_THROW(gen.get_by_ordinal_index(Ordinal(3)), std::out_of_range);
}

TEST(AppendGeneratorTest, InfiniteSequenceBehavior) {

    auto infinite_seq = LazySeq<int>([](std::size_t i) { return (int)i; }, Ordinal::omega());

    AppendGenerator<int> gen(infinite_seq, 999);

    EXPECT_EQ(gen.length(), Ordinal::omega() + Ordinal(1));

    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal::omega()), 999);
    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(500)), 500);
    EXPECT_THROW(gen.get_by_ordinal_index(Ordinal::omega() + Ordinal(1)), std::out_of_range);
}

TEST(AppendGeneratorTest, CloneBehavior) {
    int arr[] = { 1, 2 };
    LazySeq<int> source(arr, 2);
    AppendGenerator<int> gen(source, 3);

    IGenerator<int>* clone = gen.clone();


    EXPECT_EQ(clone->length(), Ordinal(3));
    EXPECT_EQ(static_cast<AppendGenerator<int>*>(clone)->get_by_ordinal_index(Ordinal(2)), 3);

    delete clone;
}