#include <gtest/gtest.h>
#include "machine_state/replace_at_generator.hpp"
#include "function_generator.hpp"
#include "lazy_sequence.hpp"
#include "ordinal.hpp"
#include <stdexcept>

char test_empty_tape_func(std::size_t index) {
    return '_';
}

char test_alphabet_func(std::size_t index) {
    return static_cast<char>('A' + (index % 26));
}

TEST(ReplaceAtGeneratorTest, FiniteSequenceBehavior) {
    Ordinal len(5);
    FunctionGenerator<char> base_gen(test_alphabet_func, len);

    ReplaceAtGenerator<char> gen(&base_gen, 'X', Ordinal(2));

    EXPECT_EQ(gen.length(), Ordinal(5));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 'A');
    EXPECT_EQ(gen.get_next(), 'B');
    EXPECT_EQ(gen.get_next(), 'X');
    EXPECT_EQ(gen.get_next(), 'D');
    EXPECT_EQ(gen.get_next(), 'E');
    EXPECT_FALSE(gen.has_next());

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 'A');
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(1)), 'B');
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(2)), 'X');
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(4)), 'E');

    EXPECT_THROW(gen.get_by_ordinal(Ordinal(5)), std::out_of_range);
}

TEST(ReplaceAtGeneratorTest, InfiniteSequenceBehavior) {
    FunctionGenerator<char> base_tape(test_empty_tape_func, Ordinal::omega());

    ReplaceAtGenerator<char> gen(&base_tape, 'X', Ordinal(2));

    EXPECT_EQ(gen.length(), Ordinal::omega());

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), '_');
    EXPECT_EQ(gen.get_next(), '_');
    EXPECT_EQ(gen.get_next(), 'X');
    EXPECT_EQ(gen.get_next(), '_');

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(2)), 'X');

    EXPECT_THROW(gen.get_by_ordinal(Ordinal(0)), std::logic_error);

    EXPECT_THROW(gen.get_by_ordinal(Ordinal::omega()), std::out_of_range);
}

TEST(ReplaceAtGeneratorTest, CloneBehavior) {
    Ordinal len(3);
    FunctionGenerator<char> base_gen(test_alphabet_func, len);
    ReplaceAtGenerator<char> gen(&base_gen, 'Z', Ordinal(1));

    IGenerator<char>* clone = gen.clone();

    EXPECT_EQ(clone->length(), Ordinal(3));
    EXPECT_EQ(static_cast<ReplaceAtGenerator<char>*>(clone)->get_by_ordinal(Ordinal(1)), 'Z');

    EXPECT_EQ(gen.get_next(), 'A');
    EXPECT_EQ(gen.get_next(), 'Z');

    EXPECT_EQ(clone->get_next(), 'A');

    delete clone;
}

TEST(ReplaceAtGeneratorTest, ConstructorValidation) {
    EXPECT_THROW(ReplaceAtGenerator<char>(nullptr, 'A', Ordinal(0)), std::logic_error);

    Ordinal len(3);
    FunctionGenerator<char> base_gen(test_alphabet_func, len);

    EXPECT_THROW(ReplaceAtGenerator<char>(&base_gen, 'X', Ordinal(3)), std::out_of_range);
}