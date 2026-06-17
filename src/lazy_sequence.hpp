#pragma once

#include "LAB2/mutable_array_sequence.hpp"
#include "LAB2/sequence.hpp"
#include "ordinal.hpp"
#include <stdexcept>
#include "i_generator.hpp"
#include "LAB2/ienumerator.hpp"
#include "function_generator.hpp"
#include "recurrent_generator.hpp"
#include "sequence_generator.hpp"
#include "map_generator.hpp"
#include "filter_generator.hpp"
#include "insert_at_generator.hpp"
#include "prepend_generator.hpp"
#include "insert_at_seq_generator.hpp"



template <typename T>
class LazySeq : public Sequence<T> {
private:
	mutable IGenerator<T>* generator_;
	mutable MutableArraySequence<T> cache_;
	bool is_infinite_;

public:

	IGenerator<T>* get_generator() const {
		return generator_;
	}
	void evaluate_up_to(int index) const;
	LazySeq();
	LazySeq(T* items, int count);
	LazySeq(const LazySeq<T>& other);
	explicit LazySeq(const IGenerator<T>* gen); 
	LazySeq(T(*func)(std::size_t), Ordinal length); // constructor for function generator
	LazySeq(T(*rule)(const MutableArraySequence<T>&),//consrtuctor for recurrent generator
		const MutableArraySequence<T>& initialValues,
		Ordinal length);
	LazySeq(const MutableArraySequence<T>& seq); // constructor for sequence generator
	
	~LazySeq();

	LazySeq<T>* clone() const override;

	int get_size() const override {
		return cache_.get_size();
	}

	bool is_infinite() const;
	const T& get(int index) const override;
	const T& get_first() const override;
	const T& get_last() const override;
	T get(const Ordinal& index) const;
	const Ordinal get_ordinal_length() const;

	LazySeq<T>* replace_at(const T& element, const Ordinal& index) const;
	LazySeq<T>* insert_at(const Ordinal& index, const LazySeq<T>& other) const;
	LazySeq<T>* prepend(const T& element) override;
	LazySeq<T>* map(T(*func)(const T&)) const;
	LazySeq<T>* where(bool (*predicate)(const T&)) const;
	LazySeq<T>* concat(const LazySeq<T>& other) const;
	LazySeq<T>* append(const T& item) override;
	LazySeq<T>* insert_at (const T& element, const Ordinal& index) const;
	LazySeq<T>* insert_at(const T& element, int index) override;

};



#include "lazy_sequence.tpp"