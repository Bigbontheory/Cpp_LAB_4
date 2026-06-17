#pragma once

#include <gtest/gtest.h>
#include <stdexcept>
#include "insert_at_seq_generator.hpp"
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

// Тесты
TEST(InsertSeqAtGeneratorTest, FiniteInsertion) {
    int b_arr[] = { 1, 2, 5, 6 };
    int i_arr[] = { 3, 4 };
    FiniteMock base(b_arr, 4);
    FiniteMock ins(i_arr, 2);

    InsertSeqAtGenerator<int> gen(&base, &ins, Ordinal(2));

    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(gen.get_next(), 3);
    EXPECT_EQ(gen.get_next(), 4);
    EXPECT_EQ(gen.get_next(), 5);
    EXPECT_EQ(gen.get_next(), 6);
}

TEST(InsertSeqAtGeneratorTest, InfiniteIntoFinite) {
    int b_arr[] = { 10, 20 };
    FiniteMock base(b_arr, 2);
    InfiniteMock ins(natural_generator_func);

    InsertSeqAtGenerator<int> gen(&base, &ins, Ordinal(1));

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 10);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(1)), 0);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(2)), 1);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega()), 20);
}

TEST(InsertSeqAtGeneratorTest, InfiniteIntoInfinite) {
    InfiniteMock base(natural_generator_func);
    InfiniteMock ins(second_natural_generator_func);

    InsertSeqAtGenerator<int> gen(&base, &ins, Ordinal(1));

    EXPECT_EQ(gen.get_by_ordinal(Ordinal(0)), 0);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(1)), 100);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal(2)), 101);
    EXPECT_EQ(gen.get_by_ordinal(Ordinal::omega()), 1);
}