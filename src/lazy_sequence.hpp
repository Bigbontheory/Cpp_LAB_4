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
#include "lazy_sequence_builder.hpp"
#include "append_generator.hpp"
#include "prepend_generator.hpp"
#include "map_generator.hpp"
#include "filter_generator.hpp"
#include "concat_generator.hpp"
#include "insert_at_generator.hpp"


template <typename T>
class LazySeq : public Sequence<T> {
private:
	mutable IGenerator<T>* generator_;
	mutable MutableArraySequence<T> cache_;
	bool is_infinite_;
	void evaluate_up_to(int index) const;

	class LazyEnumerator;
	friend class LazyEnumerator;

public:
	bool is_infinite() const;
	LazySeq();
	LazySeq(const T* items, int count);
	LazySeq(const LazySeq<T>& other);
	explicit LazySeq(const IGenerator<T>* gen); 
	LazySeq(T(*func)(std::size_t), Ordinal length); // constructor for function generator
	LazySeq(T(*rule)(const MutableArraySequence<T>&),//consrtuctor for recurrent generator
		const MutableArraySequence<T>& initialValues,
		Ordinal length);
	LazySeq(const Sequence<T>& seq); // constructor for sequence generator
	
	~LazySeq();

	LazySeq<T>* clone() const override;


	int get_size() const override {
		return cache_.get_size();
	}
	const T& get(int index) const override;
	const T& get_first() const override;
	const T& get_last() const override;
	const T& get(const Ordinal& index) const;

	LazySeq<T>* map(T(*mapper)(const T& elem)) const override;
	LazySeq<T>* where(bool(*predicate)(const T& elem)) const override;
	LazySeq<T>* concat(const LazySeq<T>& other) const;
	
	LazySeq<T>* remove_at(int index) { return nullptr; }
	LazySeq<T>* insert_at(const T& element, int index) override { return nullptr;}
	LazySeq<T>* append(const T& item) override;
	LazySeq<T>* prepend(const T& item) override;
	LazySeq<T>* insert_at_ordinal(const T& element, Ordinal pos);

	IEnumerator<T>* get_enumerator() const override;
	ISequenceBuilder<T>* create_builder() const override {
		return new LazySeqBuilder<T>();
	}

	
	class LazyEnumerator : public IEnumerator<T> {
	private:
		const LazySeq<T>* sequence_;
		int index_;
		bool is_valid_;
	public:
		LazyEnumerator(const LazySeq<T>* seq)
			: sequence_(seq), index_(-1), is_valid_(false) {
		}
		bool move_next() override {
			index_++;
			try {
				sequence_->get(index_);
				is_valid_ = true;
				return true;
			}
			catch (const std::out_of_range&) {
				is_valid_ = false;
				return false;
			}
		}
		const T& get_current() const override {
			if (!is_valid_) {
				throw std::out_of_range("Enumerator is not valid");
			}
			return sequence_->get(index_);
		}

		void reset() override {
			index_ = -1;
			is_valid_ = false;
		}
	};
};



#include "lazy_sequence.tpp"