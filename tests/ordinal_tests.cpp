#include <gtest/gtest.h>
#include <stdexcept>
#include "ordinal.hpp"


TEST(OrdinalTest, DefaultConstructor) {
    Ordinal ord;
    EXPECT_EQ(ord.get_omega_count(), 0);
    EXPECT_EQ(ord.get_finite_part(), 0);
    EXPECT_FALSE(ord.is_infinite());
}

TEST(OrdinalTest, FiniteConstructor) {
    Ordinal ord(42);
    EXPECT_EQ(ord.get_omega_count(), 0);
    EXPECT_EQ(ord.get_finite_part(), 42);
    EXPECT_FALSE(ord.is_infinite());
}

TEST(OrdinalTest, FullConstructor) {
    Ordinal ord(2, 7);
    EXPECT_EQ(ord.get_omega_count(), 2);
    EXPECT_EQ(ord.get_finite_part(), 7);
    EXPECT_TRUE(ord.is_infinite()); 
}

TEST(OrdinalTest, StaticOmega) {
    Ordinal ord = Ordinal::omega();
    EXPECT_EQ(ord.get_omega_count(), 1);
    EXPECT_EQ(ord.get_finite_part(), 0);
    EXPECT_TRUE(ord.is_infinite());  
}

TEST(OrdinalTest, EqualityOperators) {
    Ordinal ord1(2, 5);
    Ordinal ord2(2, 5);
    Ordinal ord3(1, 5);
    Ordinal ord4(2, 4);

    EXPECT_TRUE(ord1 == ord2);
    EXPECT_FALSE(ord1 == ord3);
    EXPECT_FALSE(ord1 == ord4);

    EXPECT_FALSE(ord1 != ord2);
    EXPECT_TRUE(ord1 != ord3);
    EXPECT_TRUE(ord1 != ord4);
}

TEST(OrdinalTest, RelationalOperators) {
    Ordinal ord1(1, 5);
    Ordinal ord2(1, 10);
    Ordinal ord3(2, 0);

    EXPECT_TRUE(ord1 < ord2);
    EXPECT_TRUE(ord2 < ord3);
    EXPECT_TRUE(ord1 < ord3);
    EXPECT_FALSE(ord2 < ord1);

    EXPECT_TRUE(ord1 <= ord2);
    EXPECT_TRUE(ord1 <= ord1);
    EXPECT_FALSE(ord3 <= ord2);

    EXPECT_TRUE(ord3 > ord2);
    EXPECT_FALSE(ord1 > ord2);
}

TEST(OrdinalTest, AdditionOperator) {
    Ordinal f1(5);
    Ordinal f2(10);
    Ordinal w1 = Ordinal::omega();
    Ordinal w2(2, 3);

    Ordinal res1 = f1 + f2;
    EXPECT_EQ(res1.get_omega_count(), 0);
    EXPECT_EQ(res1.get_finite_part(), 15);

    Ordinal res2 = f1 + w1;
    EXPECT_EQ(res2.get_omega_count(), 1);
    EXPECT_EQ(res2.get_finite_part(), 0);

    Ordinal res3 = w1 + f1;
    EXPECT_EQ(res3.get_omega_count(), 1);
    EXPECT_EQ(res3.get_finite_part(), 5);

    Ordinal res4 = w1 + w2;
    EXPECT_EQ(res4.get_omega_count(), 3);
    EXPECT_EQ(res4.get_finite_part(), 3);
}

TEST(OrdinalTest, DecrementOperator) {
    Ordinal ord1(0, 5);
    --ord1;
    EXPECT_EQ(ord1.get_finite_part(), 4);
    Ordinal ord2(1, 0);
    EXPECT_THROW(--ord2, std::logic_error);

    Ordinal ord3(1, 1);
    --ord3;
    EXPECT_EQ(ord3.get_omega_count(), 1);
    EXPECT_EQ(ord3.get_finite_part(), 0);
}