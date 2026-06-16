#include <gtest/gtest.h>
#include <stdexcept>
#include "filter_generator.hpp"
#include "lazy_sequence.hpp"


static bool is_even(const int& x) { return x % 2 == 0; }
static bool less_than_five(const int& x) { return x < 5; }

struct TestInfiniteGen : public IGenerator<int> {
    mutable int value;

    TestInfiniteGen(int start_val = 10) : value(start_val) {}

    Ordinal length() const override {
        return Ordinal::omega();
    }

    bool has_next() const override { return true; }

    int get_next() override { return value++; }

    IGenerator<int>* clone() const override {
        return new TestInfiniteGen(value);
    }

    int get_by_ordinal(const Ordinal& index) const override {
        throw std::logic_error("get_by_ordinal is not implemented for this test mock");
    }
};


TEST(FilterGeneratorTest, BasicFiltering) {
    int data[] = { 1, 2, 3, 4, 5, 6 };
    LazySeq<int> seq(data, 6);

    auto* filtered = seq.where(is_even);

    EXPECT_EQ(filtered->get(0), 2);
    EXPECT_EQ(filtered->get(1), 4);
    EXPECT_EQ(filtered->get(2), 6);

    delete filtered;
}

TEST(FilterGeneratorTest, InfiniteLoopProtection) {
    TestInfiniteGen inf_gen(10);
    FilterGenerator<int> filter_gen(inf_gen, less_than_five);

    EXPECT_THROW(filter_gen.has_next(), std::runtime_error);
}

TEST(FilterGeneratorTest, LengthErrorOnInfinite) {
    TestInfiniteGen inf_gen(0);
    FilterGenerator<int> filter_gen(inf_gen, is_even);

    EXPECT_THROW(filter_gen.length(), std::logic_error);
}

TEST(FilterGeneratorTest, CloneIntegrity) {
    TestInfiniteGen inf_gen(1);
    FilterGenerator<int> filter_gen(inf_gen, is_even);

    auto* clone = filter_gen.clone();

    EXPECT_TRUE(clone->has_next());
    EXPECT_EQ(clone->get_next(), 2);

    delete clone;
}