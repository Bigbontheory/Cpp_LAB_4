#include <gtest/gtest.h>
#include <stdexcept>
#include "concat_generator.hpp"
#include "ordinal.hpp"
#include "LAB2/mutable_array_sequence.hpp"

class FiniteMock : public IGenerator<int> {
private:
    MutableArraySequence<int> data_;
    int pos_ = 0;
public:
    FiniteMock(int* items, int count) : data_(items, count) {}
    FiniteMock(const FiniteMock& other) : data_(other.data_), pos_(other.pos_) {}

    int get_next() override {
        if (!has_next()) throw std::out_of_range("FiniteMock: out of bounds");
        return data_.get(pos_++);
    }
    bool has_next() const override { return pos_ < data_.get_size(); }
    Ordinal length() const override { return Ordinal(data_.get_size()); }

    int get_by_ordinal(const Ordinal& index) const override {
        if (index.is_infinite()) {
            throw std::out_of_range("FiniteMock: index out of bounds");
        }

        int finite_idx = static_cast<int>(index.get_finite_part());
        if (finite_idx >= data_.get_size() || finite_idx < 0) {
            throw std::out_of_range("FiniteMock: out of bounds");
        }
        return data_.get(finite_idx);
    }
    IGenerator<int>* clone() const override { return new FiniteMock(*this); }
};

class InfiniteMock : public IGenerator<int> {
private:
    int (*func_)(std::size_t);
    std::size_t pos_ = 0;
public:
    InfiniteMock(int (*f)(std::size_t)) : func_(f) {}

    int get_next() override { return func_(pos_++); }
    bool has_next() const override { return true; }
    Ordinal length() const override { return Ordinal::omega(); }
    int get_by_ordinal(const Ordinal& index) const override {
        return func_(index.get_finite_part());
    }
    IGenerator<int>* clone() const override { return new InfiniteMock(*this); }
};

static int natural_generator_func(std::size_t index) {
    return static_cast<int>(index);
}

static int second_natural_generator_func(std::size_t index) {
    return 100 + static_cast<int>(index);
}

class PotentialInfiniteMock : public IGenerator<int> {
private:
    int counter_ = 0;
public:
    int get_next() override {
        if (!has_next()) throw std::out_of_range("Mock: exhausted");
        return counter_++;
    }
    bool has_next() const override { return counter_ < 3; }
    Ordinal length() const override { return Ordinal::omega(); }
    int get_by_ordinal(const Ordinal& index) const override {
        return static_cast<int>(index.get_finite_part());
    }
    IGenerator<int>* clone() const override { return new PotentialInfiniteMock(*this); }
};

TEST(ConcatGeneratorTest, FiniteConcat) {
    int arr1[] = { 10, 20 };
    int arr2[] = { 30, 40 };

    FiniteMock m1(arr1, 2);
    FiniteMock m2(arr2, 2);

    ConcatGenerator<int> gen(&m1, &m2);

    EXPECT_EQ(gen.length(), Ordinal(4));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 10);
    EXPECT_EQ(gen.get_next(), 20);
    EXPECT_EQ(gen.get_next(), 30);
    EXPECT_EQ(gen.get_next(), 40);
    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);

    ConcatGenerator<int> gen_for_indices(&m1, &m2);
    EXPECT_EQ(gen_for_indices.get_by_ordinal(Ordinal(0)), 10);
    EXPECT_EQ(gen_for_indices.get_by_ordinal(Ordinal(1)), 20);
    EXPECT_EQ(gen_for_indices.get_by_ordinal(Ordinal(2)), 30);
    EXPECT_EQ(gen_for_indices.get_by_ordinal(Ordinal(3)), 40);
    EXPECT_THROW(gen_for_indices.get_by_ordinal(Ordinal(4)), std::out_of_range);
}

TEST(ConcatGeneratorTest, TransfiniteConcat) {
    InfiniteMock m1(natural_generator_func);
    int arr2[] = { 888, 999 };
    FiniteMock m2(arr2, 2);

    ConcatGenerator<int> gen(&m1, &m2);

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
    FiniteMock m1(arr1, 1);
    FiniteMock m2(arr2, 1);

    ConcatGenerator<int> gen(&m1, &m2);
    IGenerator<int>* cloned = gen.clone();

    EXPECT_EQ(cloned->length(), Ordinal(2));
    EXPECT_EQ(cloned->get_by_ordinal(Ordinal(0)), 5);
    EXPECT_EQ(cloned->get_by_ordinal(Ordinal(1)), 6);

    delete cloned;
}

TEST(ConcatGeneratorTest, TransfiniteDoubleOmegaConcat) {
    InfiniteMock m1(natural_generator_func);
    InfiniteMock m2(second_natural_generator_func);

    ConcatGenerator<int> gen(&m1, &m2);

    EXPECT_EQ(gen.length(), Ordinal::omega() + Ordinal::omega());

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 0);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(99)), 99);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega()), 100);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega() + Ordinal(5)), 105);
}

TEST(ConcatGeneratorTest, PotentialInfinityAbruptCutoff) {
    PotentialInfiniteMock mock;
    int tail_arr[] = { 777, 888 };
    FiniteMock tail(tail_arr, 2);

    ConcatGenerator<int> gen(&mock, &tail);

    EXPECT_EQ(gen.length(), Ordinal::omega() + Ordinal(2));

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 0);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 1);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 2);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 777);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 888);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}