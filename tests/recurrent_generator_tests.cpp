#include <gtest/gtest.h>
#include "recurrent_generator.hpp"
#include "LAB2/mutable_array_sequence.hpp"


int double_rule(const MutableArraySequence<int>& seq) {
    return seq.get(seq.get_size() - 1) * 2;
}


TEST(RecurrentGeneratorTest, GeneratesCorrectValues) {

    MutableArraySequence<int> init_seq;
    init_seq.append(1);
    init_seq.append(2);

    Ordinal length(5);
    RecurrentGenerator<int> gen(init_seq, double_rule, length);

    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(gen.get_next(), 4);
    EXPECT_EQ(gen.get_next(), 8);
    EXPECT_EQ(gen.get_next(), 16);
    EXPECT_FALSE(gen.has_next());
}


TEST(RecurrentGeneratorTest, ThrowsOnExhaustion) {
    MutableArraySequence<int> init_seq;
    init_seq.append(10);

    RecurrentGenerator<int> gen(init_seq, double_rule, Ordinal(1));

    gen.get_next(); 
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(RecurrentGeneratorTest, CloneWorks) {
    MutableArraySequence<int> init_seq;
    init_seq.append(1);

    RecurrentGenerator<int> gen(init_seq, double_rule, Ordinal(10));
    auto* cloned_gen = gen.clone();

    EXPECT_EQ(cloned_gen->get_next(), 1);
    delete cloned_gen;
}