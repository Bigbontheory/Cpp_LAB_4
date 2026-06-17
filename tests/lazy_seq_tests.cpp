#include <gtest/gtest.h>
#include <stdexcept>
#include "lazy_sequence.hpp"
#include "ordinal.hpp"
#include "LAB2/mutable_array_sequence.hpp"

int identity_generator(std::size_t i) {
    return static_cast<int>(i);
}

int multiply_by_two(const int& x) {
    return x * 2;
}

bool is_even(const int& x) {
    return x % 2 == 0;
}

int fibonacci_rule(const MutableArraySequence<int>& seq) {
    std::size_t n = seq.get_size();
    return seq.get(n - 1) + seq.get(n - 2);
}


static inline int my_test_function(std::size_t idx) {
    return static_cast<int>(idx);
}

TEST(LazySeqCoreTest, CloneMethodWithConstructor) {
    LazySeq<int> seq(my_test_function, Ordinal(5));
    seq.get(Ordinal(2));

    LazySeq<int>* cloned = seq.clone();

    EXPECT_EQ(cloned->get(Ordinal(2)), 2);

    EXPECT_EQ(cloned->get(Ordinal(3)), 3);
    EXPECT_EQ(cloned->get(Ordinal(4)), 4); 

    delete cloned;
}


TEST(LazySeqCoreTest, CopyConstructorTest) {
    LazySeq<int> seq(my_test_function, Ordinal(5));


    seq.get(Ordinal(2));

    LazySeq<int> copy_seq(seq);


    EXPECT_EQ(copy_seq.get_size(), 3);
}

TEST(LazySeqCoreTest, EvaluateUpToTest) {
    LazySeq<int> seq(my_test_function, Ordinal(5));

    EXPECT_EQ(seq.get_size(), 0);

    seq.evaluate_up_to(1);

    EXPECT_EQ(seq.get_size(), 2) << "Size should be 2 after evaluating up to index 1";
    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);

    seq.evaluate_up_to(4);
    EXPECT_EQ(seq.get_size(), 5);
    EXPECT_EQ(seq.get(4), 4);
}

TEST(LazySeqCoreTest, EvaluateUpToCorrectFilling) {
    int data[] = { 10, 20, 30, 40, 50 };
    LazySeq<int> seq(data, 5);

    EXPECT_EQ(seq.get(Ordinal(2)), 30);

    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(1), 20);
}

TEST(LazySeqCoreTest, EvaluateUpToBoundaryCondition) {
    int data[] = { 100, 200 };
    LazySeq<int> seq(data, 2);

    EXPECT_EQ(seq.get(Ordinal(1)), 200);
}

TEST(LazySeqCoreTest, EvaluateUpToException) {
    int data[] = { 10, 20 };
    LazySeq<int> seq(data, 2);

    EXPECT_THROW(seq.get(Ordinal(5)), std::out_of_range);
}

TEST(LazySeqConstructorTest, DefaultConstructor) {
    LazySeq<int> seq;
    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(0, 0));
    EXPECT_FALSE(seq.is_infinite());
    EXPECT_THROW(seq.get(Ordinal(0)), std::out_of_range);
}

TEST(LazySeqConstructorTest, ArrayConstructor) {
    int data[] = { 1, 2, 3 };
    LazySeq<int> seq(data, 3);
    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(0, 3));
    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(2), 3);
}

TEST(LazySeqConstructorTest, FunctionConstructor) {
    LazySeq<int> seq(identity_generator, Ordinal(0, 5));
    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(4), 4);
    EXPECT_FALSE(seq.is_infinite());
}

TEST(LazySeqConstructorTest, RecurrentConstructor) {
    int init_data[] = { 1, 1 };
    MutableArraySequence<int> init_seq(init_data, 2);
    LazySeq<int> seq(fibonacci_rule, init_seq, Ordinal(0, 5));

    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 2);
    EXPECT_EQ(seq.get(3), 3);
    EXPECT_EQ(seq.get(4), 5);
}

TEST(LazySeqConstructorTest, CopyConstructor) {
    int data[] = { 10, 20 };
    LazySeq<int> seq(data, 2);
    LazySeq<int> copy_seq(seq);

    EXPECT_EQ(copy_seq.get(0), 10);
    EXPECT_EQ(copy_seq.get(1), 20);
}

TEST(LazySeqCoreTest, JustGetTest) {
    int data[] = { 7, 8 };
    LazySeq<int> seq(data, 2);

    EXPECT_EQ(seq.get(0), 7);
    EXPECT_EQ(seq.get(1), 8);
}

TEST(LazySeqAccessTest, GetFirstAndLast) {
    int data[] = { 10, 20, 30 };
    LazySeq<int> seq(data, 3);

    EXPECT_EQ(seq.get_first(), 10);
    EXPECT_EQ(seq.get_last(), 30);
}

TEST(LazySeqAccessTest, GetByOrdinal) {
    int data[] = { 5, 15, 25 };
    LazySeq<int> seq(data, 3);

    EXPECT_EQ(seq.get(Ordinal(0, 0)), 5);
    EXPECT_EQ(seq.get(Ordinal(0, 2)), 25);
    EXPECT_THROW(seq.get(Ordinal(0, 5)), std::out_of_range);
}

TEST(LazySeqMetadataTest, LengthAndInfinity) {
    int data[] = { 1, 2 };
    LazySeq<int> finite_seq(data, 2);
    
    EXPECT_EQ(finite_seq.get_ordinal_length(), Ordinal(0, 2));
    EXPECT_FALSE(finite_seq.is_infinite());

    LazySeq<int> infinite_seq(identity_generator, Ordinal(1, 0));

    EXPECT_TRUE(infinite_seq.get_ordinal_length().is_infinite());
    EXPECT_TRUE(infinite_seq.is_infinite());
}

TEST(LazySeqModifyTest, AppendElement) {
    int data[] = { 1, 2 };
    LazySeq<int> seq(data, 2);

    LazySeq<int>* result = seq.append(99);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(0, 3));
    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 99);

    delete result;
}

TEST(LazySeqModifyTest, PrependElement) {
    int data[] = { 1, 2 };
    LazySeq<int> seq(data, 2);

    LazySeq<int>* result = seq.prepend(99);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(0, 3));
    EXPECT_EQ(result->get(0), 99);
    EXPECT_EQ(result->get(1), 1);
    EXPECT_EQ(result->get(2), 2);

    delete result;
}

TEST(LazySeqModifyTest, InsertAtInt) {
    int data[] = { 10, 30 };
    LazySeq<int> seq(data, 2);

    LazySeq<int>* result = seq.insert_at(20, 1);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);

    delete result;
}

TEST(LazySeqModifyTest, InsertAtOrdinal) {
    int data[] = { 10, 30 };
    LazySeq<int> seq(data, 2);

    LazySeq<int>* result = seq.insert_at(20, Ordinal(0, 1));

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);

    delete result;
}

TEST(LazySeqModifyTest, ConcatSequences) {
    int data1[] = { 1, 2 };
    int data2[] = { 3, 4 };
    LazySeq<int> seq1(data1, 2);
    LazySeq<int> seq2(data2, 2);

    LazySeq<int>* result = seq1.concat(seq2);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(0, 4));
    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);

    delete result;
}

TEST(LazySeqHigherOrderTest, MapFunction) {
    int data[] = { 1, 2, 3 };
    LazySeq<int> seq(data, 3);

    LazySeq<int>* result = seq.map(multiply_by_two);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(0, 3));
    EXPECT_EQ(result->get(0), 2);
    EXPECT_EQ(result->get(1), 4);
    EXPECT_EQ(result->get(2), 6);

    delete result;
}

TEST(LazySeqHigherOrderTest, WherePredicate) {
    int data[] = { 1, 2, 3, 4, 5 };
    LazySeq<int> seq(data, 5);

    LazySeq<int>* result = seq.where(is_even);

    EXPECT_EQ(result->get(0), 2);
    EXPECT_EQ(result->get(1), 4);
    EXPECT_THROW(result->get(2), std::out_of_range); 

    delete result;
}

TEST(LazySeqHigherOrderTest, WhereThrowsOnNullptr) {
    int data[] = { 1 };
    LazySeq<int> seq(data, 1);

    EXPECT_THROW(seq.where(nullptr), std::invalid_argument);
}


TEST(LazySeqInsertAtTest, InsertInMiddle) {
    int base_data[] = { 1, 2, 5, 6 };
    int insert_data[] = { 3, 4 };

    LazySeq<int> base(base_data, 4);
    LazySeq<int> to_insert(insert_data, 2);

    LazySeq<int>* result = base.insert_at(Ordinal(2), to_insert);

    EXPECT_EQ(result->get_ordinal_length().get_finite_part(), 6);

    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);
    EXPECT_EQ(result->get(4), 5);
    EXPECT_EQ(result->get(5), 6);

    delete result;
}

TEST(LazySeqInsertAtTest, InsertAtBegin) {
    int base_data[] = { 10, 20 };
    int insert_data[] = { 1, 2 };

    LazySeq<int> base(base_data, 2);
    LazySeq<int> to_insert(insert_data, 2);

    LazySeq<int>* result = base.insert_at(Ordinal(0), to_insert);

    EXPECT_EQ(result->get_ordinal_length().get_finite_part(), 4);
    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 10);
    EXPECT_EQ(result->get(3), 20);

    delete result;
}


TEST(LazySeqInsertAtTest, InsertAtEnd) {
    int base_data[] = { 1, 2 };
    int insert_data[] = { 3, 4 };

    LazySeq<int> base(base_data, 2);
    LazySeq<int> to_insert(insert_data, 2);

    LazySeq<int>* result = base.insert_at(Ordinal(2), to_insert);

    EXPECT_EQ(result->get_ordinal_length().get_finite_part(), 4);
    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);

    delete result;
}

TEST(LazySeqInsertAtTest, IndexOutOfBoundsThrows) {
    int data[] = { 1, 2 };
    LazySeq<int> base(data, 2);
    LazySeq<int> to_insert(data, 2);

    EXPECT_THROW(base.insert_at(Ordinal(3), to_insert), std::out_of_range);
}

char empty_tape(std::size_t index) {
    return '_';
}


TEST(LazySeqReplaceTest, ReplaceAtIntegrity) {
    LazySeq<char> base_seq(empty_tape, Ordinal::omega());

    LazySeq<char>* replaced_seq = base_seq.replace_at('X', Ordinal(10));

    EXPECT_EQ(base_seq.get(0), '_');
    EXPECT_EQ(base_seq.get(1), '_');

    EXPECT_EQ(replaced_seq->get(Ordinal(10)), 'X');
    EXPECT_EQ(replaced_seq->get(Ordinal(9)), '_');
    EXPECT_EQ(replaced_seq->get(Ordinal(11)), '_');

    delete replaced_seq;
}

TEST(LazySeqReplaceTest, ChainOfReplacements) {
    LazySeq<char> base_seq(empty_tape, Ordinal::omega());

    LazySeq<char>* step1 = base_seq.replace_at('A', Ordinal(0));
    LazySeq<char>* step2 = step1->replace_at('B', Ordinal(1));

    EXPECT_EQ(step2->get(Ordinal(0)), 'A');
    EXPECT_EQ(step2->get(Ordinal(1)), 'B');
    EXPECT_EQ(step2->get(Ordinal(2)), '_');

    delete step1;
    delete step2;
}

TEST(LazySeqReplaceTest, OutOfRange) {
    LazySeq<char> base_seq(empty_tape, Ordinal(5));

    EXPECT_THROW(base_seq.replace_at('X', Ordinal(5)), std::out_of_range);
}