#include <gtest/gtest.h>
#include <stdexcept>
#include "lazy_sequence.hpp"
#include "LAB2/mutable_array_sequence.hpp"
#include "ordinal.hpp"

static int natural_generator_func(std::size_t index) {
    return static_cast<int>(index);
}

static int square_func(std::size_t index) {
    return static_cast<int>(index * index);
}

static int fib_rule(const MutableArraySequence<int>& window) {
    return window.get(0) + window.get(1);
}

int map_triple(const int& x) {
    return x * 3;
}

int map_add_fifty(const int& x) {
    return x + 50;
}

bool is_even(const int& x) {
    return x % 2 == 0;
}

TEST(LazySeqTest, AppendChainTest) {
    int initial_items[] = { 0, 1, 2 };
    LazySeq<int> seq(initial_items, 3);

    LazySeq<int>* extended_seq = seq.append(3)->append(4);

    EXPECT_EQ(extended_seq->get_ordinal_length(), Ordinal(5));

    EXPECT_EQ(extended_seq->get(Ordinal(0)), 0);
    EXPECT_EQ(extended_seq->get(Ordinal(2)), 2);
    EXPECT_EQ(extended_seq->get(Ordinal(3)), 3);
    EXPECT_EQ(extended_seq->get(Ordinal(4)), 4);

    EXPECT_THROW(extended_seq->get(Ordinal(5)), std::out_of_range);

    delete extended_seq;
}

TEST(LazySeqTest, InfiniteLazyAccessTest) {
    LazySeq<int> infinite_seq(natural_generator_func, Ordinal::omega());

    LazySeq<int>* omega_plus_one = infinite_seq.append(999);
    EXPECT_EQ(omega_plus_one->get(Ordinal(100)), 100);
    EXPECT_EQ(omega_plus_one->get(Ordinal::omega()), 999);

    delete omega_plus_one;
}

TEST(LazySeqTest, MapFinite) {
    int data[] = { 1, 2, 3 };
    LazySeq<int> seq(data, 3);

    LazySeq<int>* mapped = seq.map(map_triple);

    EXPECT_EQ(mapped->get_ordinal_length(), Ordinal(3));
    EXPECT_EQ(mapped->get(Ordinal(0)), 3);
    EXPECT_EQ(mapped->get(Ordinal(1)), 6);
    EXPECT_EQ(mapped->get(Ordinal(2)), 9);
    EXPECT_THROW(mapped->get(Ordinal(3)), std::out_of_range);

    delete mapped;
}

TEST(LazySeqTest, MapTransfinite) {
    LazySeq<int> infinite_base(natural_generator_func, Ordinal::omega());
    LazySeq<int>* transfinite_seq = infinite_base.append(1000);

    LazySeq<int>* mapped = transfinite_seq->map(map_add_fifty);

    EXPECT_EQ(mapped->get_ordinal_length(), Ordinal::omega() + Ordinal(1));
    EXPECT_EQ(mapped->get(Ordinal(10)), 60);
    EXPECT_EQ(mapped->get(Ordinal::omega()), 1050);
    EXPECT_THROW(mapped->get(Ordinal::omega() + Ordinal(1)), std::out_of_range);

    delete mapped;
    delete transfinite_seq;
}

TEST(LazySeqTest, BasicFiltering) {
    int raw_arr[] = { 1, 2, 3, 4, 5, 6 };

    MutableArraySequence<int> source_seq(raw_arr, 6);
    LazySeq<int> seq(source_seq);

    LazySeq<int>* filtered = seq.where(is_even);
    EXPECT_EQ(filtered->get_ordinal_length(), Ordinal(3));
    EXPECT_EQ(filtered->get(Ordinal(0)), 2);
    EXPECT_EQ(filtered->get(Ordinal(1)), 4);
    EXPECT_EQ(filtered->get(Ordinal(2)), 6);

    delete filtered;
}

TEST(LazySeqTest, EmptyFilterResult) {
    int raw_arr[] = { 1, 3, 5 };
    MutableArraySequence<int> source_seq(raw_arr, 3);

    LazySeq<int> seq(source_seq);

    LazySeq<int>* filtered = seq.where(is_even);

    EXPECT_EQ(filtered->get_ordinal_length(), Ordinal(0));
    EXPECT_THROW(filtered->get(Ordinal(0)), std::out_of_range);
    delete filtered;
}

TEST(LazySeqConstructorTest, FunctionGeneratorTesting) {
    LazySeq<int> seq(square_func, Ordinal(3));

    EXPECT_EQ(seq.get(Ordinal(0)), 0);
    EXPECT_EQ(seq.get(Ordinal(1)), 1);
    EXPECT_EQ(seq.get(Ordinal(2)), 4);
    EXPECT_THROW(seq.get(Ordinal(3)), std::out_of_range);
}

TEST(LazySeqConstructorTest, RecurrentGeneratorTesting) {
    int initial_data[] = { 0, 1 };
    MutableArraySequence<int> initial_seq(initial_data, 2);

    LazySeq<int> seq(fib_rule, initial_seq, Ordinal(5));

    EXPECT_EQ(seq.get(Ordinal(0)), 0);
    EXPECT_EQ(seq.get(Ordinal(1)), 1);
    EXPECT_EQ(seq.get(Ordinal(2)), 1);
    EXPECT_EQ(seq.get(Ordinal(3)), 2);
    EXPECT_EQ(seq.get(Ordinal(4)), 3);
    EXPECT_THROW(seq.get(Ordinal(5)), std::out_of_range);
}

TEST(LazySeqConstructorTest, SequenceGeneratorConstructorTesting) {
    int raw_data[] = { 5, 10, 15 };
    MutableArraySequence<int> source(raw_data, 3);

    LazySeq<int> seq(source);

    EXPECT_EQ(seq.get(Ordinal(0)), 5);
    EXPECT_EQ(seq.get(Ordinal(1)), 10);
    EXPECT_EQ(seq.get(Ordinal(2)), 15);

    EXPECT_THROW(seq.get(Ordinal(3)), std::out_of_range);
}

TEST(LazySeqConstructorTest, DefaultConstructor) {
    LazySeq<int> seq;

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(0));
    EXPECT_THROW(seq.get(Ordinal(0)), std::out_of_range);
}


TEST(LazySeqConstructorTest, ArrayConstructor) {
    int data[] = { 10, 20, 30, 40 };
    int count = 4;

    LazySeq<int> seq(data, count);

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(count));
    EXPECT_EQ(seq.get(Ordinal(0)), 10);
    EXPECT_EQ(seq.get(Ordinal(1)), 20);
    EXPECT_EQ(seq.get(Ordinal(3)), 40);

    EXPECT_THROW(seq.get(Ordinal(4)), std::out_of_range);
}

TEST(LazySeqMethodTest, AccessorAndBoundsTest) {
    int data[] = { 10, 20, 30, 40, 50 };
    LazySeq<int> seq(data, 5);

    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(2), 30);
    EXPECT_EQ(seq.get(4), 50);

    EXPECT_EQ(seq.get_first(), 10);
    EXPECT_EQ(seq.get_last(), 50);

    EXPECT_EQ(seq.get(Ordinal(1)), 20);
    EXPECT_EQ(seq.get(Ordinal(3)), 40);

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(5));
}

TEST(LazySeqMethodTest, ErrorHandlingTest) {
    int data[] = { 10, 20 };
    LazySeq<int> seq(data, 2);

    EXPECT_THROW(seq.get(-1), std::out_of_range);
    EXPECT_THROW(seq.get(2), std::out_of_range);
    EXPECT_THROW(seq.get(100), std::out_of_range);
    EXPECT_THROW(seq.get(Ordinal(-1)), std::out_of_range);
    EXPECT_THROW(seq.get(Ordinal(2)), std::out_of_range);
}

TEST(LazySeqMethodTest, SingleElementTest) {
    int data[] = { 99 };
    LazySeq<int> seq(data, 1);

    EXPECT_EQ(seq.get_first(), 99);
    EXPECT_EQ(seq.get_last(), 99);
    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(1));
    EXPECT_EQ(seq.get(0), 99);
}
