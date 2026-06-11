#include <gtest/gtest.h>
#include "sequence_generator.hpp"
#include "LAB2/mutable_array_sequence.hpp" 
#include "ordinal.hpp"

class SequenceGeneratorTest : public ::testing::Test {
protected:
    int data[3] = { 10, 20, 30 };
    MutableArraySequence<int> seq{ data, 3 };
    SequenceGenerator<int> generator{ seq };
};


TEST_F(SequenceGeneratorTest, GetNextSequence) {
    ASSERT_TRUE(generator.has_next());
    EXPECT_EQ(generator.get_next(), 10);

    ASSERT_TRUE(generator.has_next());
    EXPECT_EQ(generator.get_next(), 20);

    ASSERT_TRUE(generator.has_next());
    EXPECT_EQ(generator.get_next(), 30);

    EXPECT_FALSE(generator.has_next());
}


TEST_F(SequenceGeneratorTest, OutOfRangeThrows) {

    generator.get_next();
    generator.get_next();
    generator.get_next();


    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST_F(SequenceGeneratorTest, CloneCreatesCopy) {
    IGenerator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);
    EXPECT_TRUE(clone->has_next());
    EXPECT_EQ(clone->get_next(), 10);

    delete clone;
}


TEST_F(SequenceGeneratorTest, LengthReturnsCorrectOrdinal) {
    Ordinal len = generator.length();
    EXPECT_EQ(len.get_value(), 3);
    EXPECT_FALSE(len.is_infinite());
}


TEST_F(SequenceGeneratorTest, GetByOrdinalValid) {
    Ordinal idx(false, 1);
    EXPECT_EQ(generator.get_by_ordinal(idx), 20);
}


TEST_F(SequenceGeneratorTest, GetByOrdinalInvalid) {
    Ordinal inf(true, 0);
    EXPECT_THROW(generator.get_by_ordinal(inf), std::out_of_range);


    Ordinal out(false, 100);
    EXPECT_THROW(generator.get_by_ordinal(out), std::out_of_range);
}


TEST_F(SequenceGeneratorTest, AssignmentOperator) {
    SequenceGenerator<int> generator2{ seq };
    generator.get_next();

    generator2 = generator;


    EXPECT_TRUE(generator2.has_next());
    EXPECT_EQ(generator2.get_next(), 20);
}