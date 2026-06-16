#pragma once

#include "dynamicarray.hpp"
#include "linkedlist.hpp"
#include "sequence.hpp"

template <typename T>
class ArraySequence : public Sequence_LAB_2<T> {
protected:
    DynamicArray<T>* items;
    virtual ArraySequence<T>* instance() = 0;

    Sequence_LAB_2<T>* append_internal(const T& item);
    Sequence_LAB_2<T>* prepend_internal(const T& item);
    Sequence_LAB_2<T>* insert_at_internal(const T& item, int index);
    Sequence_LAB_2<T>* remove_at_internal(int index);

public:
    
    template <typename U, typename SeqType>
    friend class SequenceBuilder;

    explicit ArraySequence(int size);
    ArraySequence();
    ArraySequence(T* items, int count);
    explicit ArraySequence(const LinkedList<T>& list);
    ArraySequence(const ArraySequence<T>& other);

    virtual ~ArraySequence();

    const T& get_first() const override;
    const T& get_last() const override;
    const T& get(int index) const override;
    int get_size() const override;

    Sequence_LAB_2<T>* append(const T& item) override;
    Sequence_LAB_2<T>* prepend(const T& item) override;
    Sequence_LAB_2<T>* insert_at(const T& item, int index) override;
    Sequence_LAB_2<T>* remove_at(int index) override;

    IEnumerator<T>* get_enumerator() const override;
};

#include "arraysequence.tpp"
