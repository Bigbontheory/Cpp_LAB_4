#pragma once

#include "mutable_array_sequence.hpp"
#include "sequence.hpp"
#include "ordinal.hpp"
#include <stdexcept>
#include "i_generator.hpp"
#include "ienumerator.hpp"
#include "function_generator.hpp"

template <typename T>
class LazySeq : public Sequence<T> {
private:
	mutable IGenerator<T>* generator_;
	mutable MutableArraySequence<T>* cache_;

	bool is_infinite() const;
	int get_generator_length() const;
	void evaluate_up_to(int index) const;

public:
	LazySeq();
	LazySeq(const T* items, int count);
	explicit LazySequence(const Sequence<T>& seq);
	explicit LazySeq(const IGenerator<T>& gen); 
	LazySeq(T(*func)(std::size_t), Ordinal length); // constructor for function generator
	
	~LazySeq();

	LazySeq<T>* clone() const override;

	const T& get(int index) const override;
	const T& get_first() const override;
	const T& get_last() const override;

	LazySeq<T>* map(T(*mapper))(const T& elem)) const override;
	LazySeq<T>* where(bool(*predicate)(const T& elem)) const override;
	LazySeq<T>* concat(const Sequence<T>* other) const override;

	T reduce(T(*reducer)(const T&, const T&), const T& initial_value) const override;

	LazySeq<T>* append(const T& item) override;
	LazySeq<T>* prepend(const T& item) override;
	LazySeq<T>* insert_at(const T& item, int index) override;
	LazySeq<T>* remove_at(int index) override;
	LazySeq<T>* get_subsequence(int start_index, int end_index) const override;

	IEnumerator<T>* get_enumerator() const override;

	class Enumerator : public IEnumerator<T> {
	private:
		const LazySeq<T>* sequence_;
		int index_;
		bool is_valid_;

	public:
		explicit Enumerator(const LazySeq<T>* sequence);
		bool MoveNext() override;
		const T& GetCurrent() const override;
		void Reset() override;
	};
};