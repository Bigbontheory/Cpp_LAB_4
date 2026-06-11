#include <gtest/gtest.h>
#include "lazy_sequence.hpp"
#include "LAB2/mutable_array_sequence.hpp" 

int test_func(std::size_t idx) { return static_cast<int>(idx * 10); }
int square(const int& x) { return x * x; }
bool is_even(const int& x) { return x % 2 == 0; }

class LazySeqTest : public ::testing::Test {
protected:
    // Чтобы не плодить мусор в каждом тесте, можно инициализировать здесь
};

// 1. КОНСТРУКТОРЫ
TEST_F(LazySeqTest, Constructors) {
    // Function constructor
    LazySeq<int> s1(test_func, Ordinal(5));
    EXPECT_EQ(s1.get(0), 0);
    EXPECT_EQ(s1.get(4), 40);

    // Array constructor
    int arr[] = { 1, 2, 3 };
    LazySeq<int> s2(arr, 3);
    EXPECT_EQ(s2.get(0), 1);
    EXPECT_EQ(s2.get(2), 3);
}

// 2. ДОСТУП И КЭШИРОВАНИЕ
TEST_F(LazySeqTest, AccessAndCaching) {
    LazySeq<int> seq(test_func, Ordinal(10));

    // Проверка evaluate_up_to через get
    EXPECT_EQ(seq.get(5), 50);

    // Проверка на ошибки
    EXPECT_THROW(seq.get(-1), std::out_of_range);
    EXPECT_THROW(seq.get(20), std::out_of_range);
}

// 3. GET_FIRST / GET_LAST
TEST_F(LazySeqTest, FirstLast) {
    LazySeq<int> seq(test_func, Ordinal(5));
    EXPECT_EQ(seq.get_first(), 0);
    EXPECT_EQ(seq.get_last(), 40);

    LazySeq<int> inf(test_func, Ordinal::omega());
    EXPECT_THROW(inf.get_last(), std::logic_error);
}

// 4. ТРАНСФОРМАЦИИ (MAP, WHERE)
TEST_F(LazySeqTest, MapWhere) {
    LazySeq<int> base(test_func, Ordinal(5)); // 0, 10, 20, 30, 40

    LazySeq<int>* mapped = base.map(square);
    EXPECT_EQ(mapped->get(1), 100); // 10*10

    LazySeq<int>* filtered = mapped->where(is_even);
    EXPECT_EQ(filtered->get(0), 0);
    EXPECT_EQ(filtered->get(1), 100);

    delete mapped;
    delete filtered;
}

// 5. APPEND / PREPEND
TEST_F(LazySeqTest, AppendPrepend) {
    int arr[] = { 10, 20 };
    LazySeq<int> seq(arr, 2);

    LazySeq<int>* app = seq.append(30);
    EXPECT_EQ(app->get(2), 30);

    LazySeq<int>* pre = app->prepend(5);
    EXPECT_EQ(pre->get(0), 5);
    EXPECT_EQ(pre->get(1), 10);

    delete app;
    delete pre;
}

// 6. CONCAT / INSERT_AT
TEST_F(LazySeqTest, ConcatInsert) {
    int a1[] = { 1 };
    int a2[] = { 2 };
    LazySeq<int> s1(a1, 1);
    LazySeq<int> s2(a2, 1);

    LazySeq<int>* cat = s1.concat(s2);
    EXPECT_EQ(cat->get(0), 1);
    EXPECT_EQ(cat->get(1), 2);

    LazySeq<int>* ins = cat->insert_at_ordinal(99, Ordinal(1));
    EXPECT_EQ(ins->get(1), 99);
    EXPECT_EQ(ins->get(2), 2);

    delete cat;
    delete ins;
}


TEST_F(LazySeqTest, CloneAndState) {
    LazySeq<int> orig(test_func, Ordinal(5));
    LazySeq<int>* clone = orig.clone();

    EXPECT_EQ(clone->get(0), orig.get(0));
    EXPECT_EQ(clone->is_infinite(), orig.is_infinite());

    delete clone;
}

// 8. ORDINAL GET
TEST_F(LazySeqTest, OrdinalAccess) {
    LazySeq<int> seq(test_func, Ordinal(10));
    EXPECT_EQ(seq.get(Ordinal(5)), 50);
}

// 9. ENUMERATOR
TEST_F(LazySeqTest, Enumerator) {
    LazySeq<int> seq(test_func, Ordinal(3));
    auto* en = seq.get_enumerator();
    // Проверка, что он хотя бы создается без падений
    EXPECT_NE(en, nullptr);
    delete en;
}