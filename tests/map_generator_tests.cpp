#include <gtest/gtest.h>
#include "map_generator.hpp"
#include "lazy_sequence.hpp"
#include "ordinal.hpp"

static int natural_gen(std::size_t index) {
    return static_cast<int>(index);
}

static int multiply_by_two(const int& x) {
    return x * 2;
}

static int plus_one_hundred(const int& x) {
    return x + 100;
}

TEST(MapGeneratorTest, FiniteSequenceMapping) {
    int data[] = { 10, 20, 30 };
    LazySeq<int> source(data, 3);

    MapGenerator<int> gen(source, multiply_by_two);

    EXPECT_EQ(gen.length(), Ordinal(3));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 20);
    EXPECT_EQ(gen.get_next(), 40);
    EXPECT_EQ(gen.get_next(), 60);
    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);

    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(0)), 20);
    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(1)), 40);
    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(2)), 60);
    EXPECT_THROW(gen.get_by_ordinal_index(Ordinal(3)), std::out_of_range);
}

TEST(MapGeneratorTest, TransfiniteSequenceMapping) {
    LazySeq<int> infinite_base(natural_gen, Ordinal::omega());
    LazySeq<int>* transfinite_source = infinite_base.append(500);

    MapGenerator<int> gen(*transfinite_source, plus_one_hundred);

    EXPECT_EQ(gen.length(), Ordinal::omega() + Ordinal(1));

    EXPECT_TRUE(gen.has_next());

    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal(50)), 150);

    EXPECT_EQ(gen.get_by_ordinal_index(Ordinal::omega()), 600);

    EXPECT_THROW(gen.get_by_ordinal_index(Ordinal::omega() + Ordinal(1)), std::out_of_range);

    delete transfinite_source;
}

TEST(MapGeneratorTest, CloneBehavior) {
    int data[] = { 5 };
    LazySeq<int> source(data, 1);
    MapGenerator<int> gen(source, multiply_by_two);

    IGenerator<int>* cloned_gen = gen.clone();

    EXPECT_EQ(cloned_gen->length(), Ordinal(1));
    EXPECT_EQ(static_cast<MapGenerator<int>*>(cloned_gen)->get_by_ordinal_index(Ordinal(0)), 10);

    delete cloned_gen;
}