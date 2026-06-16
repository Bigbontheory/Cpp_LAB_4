#include <gtest/gtest.h>
#include <stdexcept>
#include "lazy_sequence.hpp"
#include "ordinal.hpp"

TEST(LazySeqInsertAtTest, InsertAtBeginningInt) {
    int data[] = { 1, 2, 3 };
    LazySeq<int> seq(data, 3);

    LazySeq<int>* result = seq.insert_at(99, 0);

    EXPECT_EQ(result->get(0), 99);
    EXPECT_EQ(result->get(1), 1);
    EXPECT_EQ(result->get(2), 2);
    EXPECT_EQ(result->get(3), 3);

    delete result;
}

TEST(LazySeqInsertAtTest, InsertInMiddleInt) {
    int data[] = { 10, 20, 30 };
    LazySeq<int> seq(data, 3);

    LazySeq<int>* result = seq.insert_at(25, 2);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 25);
    EXPECT_EQ(result->get(3), 30);

    delete result;
}

TEST(LazySeqInsertAtTest, InsertAtEndInt) {
    int data[] = { 5, 6, 7 };
    LazySeq<int> seq(data, 3);

    LazySeq<int>* result = seq.insert_at(8, 3);

    EXPECT_EQ(result->get(0), 5);
    EXPECT_EQ(result->get(1), 6);
    EXPECT_EQ(result->get(2), 7);
    EXPECT_EQ(result->get(3), 8);

    delete result;
}

TEST(LazySeqInsertAtTest, InsertAtThrowsIfOutOfBoundsInt) {
    int data[] = { 1, 2 };
    LazySeq<int> seq(data, 2);

    EXPECT_THROW({
        LazySeq<int>*result = seq.insert_at(99, 3);
        delete result;
        }, std::out_of_range);
}

TEST(LazySeqInsertAtTest, ChainInsertionsInt) {
    int data[] = { 100 };
    LazySeq<int> seq(data, 1);

    LazySeq<int>* first_step = seq.insert_at(50, 0);
    LazySeq<int>* result = first_step->insert_at(150, 2);

    EXPECT_EQ(result->get(0), 50);
    EXPECT_EQ(result->get(1), 100);
    EXPECT_EQ(result->get(2), 150);

    delete first_step;
    delete result;
}

TEST(LazySeqInsertAtTest, InsertWithOrdinal) {
    int data[] = { 10, 20 };
    LazySeq<int> seq(data, 2);

    LazySeq<int>* result = seq.insert_at(99, Ordinal(0, 1));

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 99);
    EXPECT_EQ(result->get(2), 20);

    delete result;
}

TEST(LazySeqInsertAtTest, InsertAtThrowsIfOutOfBoundsOrdinal) {
    int data[] = { 10, 20 };
    LazySeq<int> seq(data, 2);

    EXPECT_THROW({
        LazySeq<int>*result = seq.insert_at(99, Ordinal(0, 5));
        delete result;
        }, std::out_of_range);
}

