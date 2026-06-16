#include <gtest/gtest.h>
#include "concat_generator.hpp"
#include "lazy_sequence.hpp"
#include "ordinal.hpp"

static int natural_generator_func(std::size_t index) {
    return static_cast<int>(index);
}

TEST(ConcatGeneratorTest, FiniteConcat) {
    int arr1[] = { 10, 20 };
    int arr2[] = { 30, 40 };
    LazySeq<int> s1(arr1, 2);
    LazySeq<int> s2(arr2, 2);

    ConcatGenerator<int> gen(s1, s2);

    EXPECT_EQ(gen.length(), Ordinal(4));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);
    EXPECT_EQ(gen.get_next(), 20);
    EXPECT_EQ(gen.get_next(), 30);
    EXPECT_EQ(gen.get_next(), 40);
    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 10);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(1)), 20);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(2)), 30);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(3)), 40);
    EXPECT_THROW(gen.get_by_ordinal(Ordinal(4)), std::out_of_range);
}

TEST(ConcatGeneratorTest, TransfiniteConcat) {
    LazySeq<int> infinite_base(natural_generator_func, Ordinal::omega());
    int arr[] = { 888, 999 };
    LazySeq<int> finite_tail(arr, 2);

    ConcatGenerator<int> gen(infinite_base, finite_tail);

    EXPECT_EQ(gen.length(), Ordinal::omega() + Ordinal(2));

    EXPECT_TRUE(gen.has_next());

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 0);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(50)), 50);

    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega()), 888);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega() + Ordinal(1)), 999);

    EXPECT_THROW(gen.get_by_ordinal(Ordinal::omega() + Ordinal(2)), std::out_of_range);
}

TEST(ConcatGeneratorTest, CloneBehavior) {
    int arr1[] = { 5 };
    int arr2[] = { 6 };
    LazySeq<int> s1(arr1, 1);
    LazySeq<int> s2(arr2, 1);

    ConcatGenerator<int> gen(s1, s2);
    IGenerator<int>* cloned = gen.clone();

    EXPECT_EQ(cloned->length(), Ordinal(2));
    EXPECT_EQ(static_cast<ConcatGenerator<int>*>(cloned)->get_by_ordinal(Ordinal(0)), 5);
    EXPECT_EQ(static_cast<ConcatGenerator<int>*>(cloned)->get_by_ordinal(Ordinal(1)), 6);

    delete cloned;
}