#include <gtest/gtest.h>
#include "concat_generator.hpp"
#include "lazy_sequence.hpp"
#include "ordinal.hpp"

static int natural_generator_func(std::size_t index) {
    return static_cast<int>(index);
}

static int second_natural_generator_func(std::size_t index) {
    
    Ordinal ord(index);
    return 100 + static_cast<int>(ord.get_finite_part());
}

TEST(ConcatGeneratorTest, FiniteConcat) {
    int arr1[] = { 10, 20 };
    int arr2[] = { 30, 40 };
    LazySeq<int> s1(arr1, 2);
    LazySeq<int> s2(arr2, 2);

    ConcatGenerator<int> gen(s1, s2);

    EXPECT_EQ(gen.length(), Ordinal(4));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);
    EXPECT_EQ(gen.get_next(), 20);
    EXPECT_EQ(gen.get_next(), 30);
    EXPECT_EQ(gen.get_next(), 40);
    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 10);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(1)), 20);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(2)), 30);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(3)), 40);
    EXPECT_THROW(gen.get_by_ordinal(Ordinal(4)), std::out_of_range);
}

TEST(ConcatGeneratorTest, TransfiniteConcat) {
    LazySeq<int> infinite_base(natural_generator_func, Ordinal::omega());
    int arr[] = { 888, 999 };
    LazySeq<int> finite_tail(arr, 2);

    ConcatGenerator<int> gen(infinite_base, finite_tail);

    EXPECT_EQ(gen.length(), Ordinal::omega() + Ordinal(2));

    EXPECT_TRUE(gen.has_next());

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 0);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(50)), 50);

    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega()), 888);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega() + Ordinal(1)), 999);

    EXPECT_THROW(gen.get_by_ordinal(Ordinal::omega() + Ordinal(2)), std::out_of_range);
}

TEST(ConcatGeneratorTest, CloneBehavior) {
    int arr1[] = { 5 };
    int arr2[] = { 6 };
    LazySeq<int> s1(arr1, 1);
    LazySeq<int> s2(arr2, 1);

    ConcatGenerator<int> gen(s1, s2);
    IGenerator<int>* cloned = gen.clone();

    EXPECT_EQ(cloned->length(), Ordinal(2));
    EXPECT_EQ(static_cast<ConcatGenerator<int>*>(cloned)->get_by_ordinal(Ordinal(0)), 5);
    EXPECT_EQ(static_cast<ConcatGenerator<int>*>(cloned)->get_by_ordinal(Ordinal(1)), 6);

    delete cloned;
}

TEST(ConcatGeneratorTest, TransfiniteDoubleOmegaConcat) {
    // Две бесконечные последовательности: каждая возвращает свое значение * 1
    // Пусть first_base возвращает 0, 1, 2...
    // Пусть second_base возвращает 100, 101, 102...
    LazySeq<int> first_base(natural_generator_func, Ordinal::omega());
    LazySeq<int> second_base(second_natural_generator_func, Ordinal::omega());

    ConcatGenerator<int> gen(first_base, second_base);

    // 1. Проверяем длину: omega + omega
    EXPECT_EQ(gen.length(), Ordinal::omega() + Ordinal::omega());

    // 2. Проверяем элементы первой последовательности (до omega)
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 0);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(99)), 99);

    // 3. Проверяем элементы второй последовательности (переход через omega)
    // omega - это начало второй последовательности (0-й элемент второй)
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega()), 100);

    // omega + 5 - это 5-й элемент второй последовательности
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega() + Ordinal(5)), 105);

    // 4. Проверяем, что нет "ошибки смещения" (значения не перепутаны)
}