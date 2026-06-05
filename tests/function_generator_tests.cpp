#include <gtest/gtest.h>
#include <functional>
#include <stdexcept>
#include "function_generator.hpp"

class FunctionGeneratorTest : public ::testing::Test {
protected:
    std::function<int(std::size_t)> test_func = [](std::size_t idx) {
        return static_cast<int>(idx * 10);
        };
};

TEST_F(FunctionGeneratorTest, ThrowsIfFunctionIsNull) {
    Ordinal length(5);
    EXPECT_THROW(FunctionGenerator<int>(nullptr, length), std::invalid_argument);
}

TEST_F(FunctionGeneratorTest, HandlesFiniteSequence) {
    Ordinal length(2);
    FunctionGenerator<int> gen(test_func, length);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 0);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST_F(FunctionGeneratorTest, HandlesInfiniteSequence) {
    Ordinal infinite_length = Ordinal::omega();
    FunctionGenerator<int> gen(test_func, infinite_length);

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_TRUE(gen.has_next());
        EXPECT_EQ(gen.get_next(), static_cast<int>(i * 10));
    }
}

TEST_F(FunctionGeneratorTest, ReturnsCorrectLength) {
    Ordinal expected_length(42);
    FunctionGenerator<int> gen(test_func, expected_length);

    Ordinal actual_length = gen.length();

    EXPECT_EQ(actual_length.get_finite_part(), expected_length.get_finite_part());
    EXPECT_EQ(actual_length.get_omega_count(), expected_length.get_omega_count());
}

TEST_F(FunctionGeneratorTest, CloneCreatesIndependentCopy) {
    Ordinal length(5);
    FunctionGenerator<int> original(test_func, length);

    original.get_next();

    FunctionGenerator<int>* cloned = original.clone();

    EXPECT_TRUE(cloned->has_next());
    EXPECT_EQ(cloned->get_next(), 10);

    cloned->get_next();

    EXPECT_EQ(original.get_next(), 10);

    delete cloned;
}