#include "concat_generator.hpp"
#include "append_generator.hpp"

template<typename T>
LazySeq<T>::LazySeq(T(*func)(std::size_t), Ordinal length) {
	if (func == nullptr) {
		throw std::invalid_argument("func cannot be nullptr");
	}
	generator_ = new FunctionGenerator<T>(func, length);
	is_infinite_ = length.is_infinite();
}

template <typename T>
LazySeq<T>::LazySeq(const LazySeq<T>& other) {
	is_infinite_ = other.is_infinite_;
	cache_ = other.cache_;

	if (other.generator_) {
		generator_ = other.generator_->clone();
	}
	else {
		generator_ = nullptr;
	}
}

template <typename T>
LazySeq<T>::LazySeq(const T* items, int count) : is_infinite_(false) {
	for (int i = 0; i < count; ++i) {
		cache_.append(items[i]);
	}
	generator_ = new SequenceGenerator<T>(cache_);
}

template <typename T>
LazySeq<T>::LazySeq(const IGenerator<T>* gen)
	: generator_(gen->clone()), is_infinite_(generator_->length().is_infinite()) {
}

template <typename T>
LazySeq<T>* LazySeq<T>::clone() const {
	LazySeq<T>* new_seq = new LazySeq<T>();

	new_seq->is_infinite_ = this->is_infinite_;

	if (this->generator_) {
		new_seq->generator_ = this->generator_->clone();
	}

	new_seq->cache_ = this->cache_;

	return new_seq;
}

template <typename T>
LazySeq<T>::LazySeq(T(*rule)(const MutableArraySequence<T>&),
	const MutableArraySequence<T>& initialValues,
	Ordinal length)
{
	generator_ = new RecurrentGenerator<T>(initialValues, rule, length);
	is_infinite_ = length.is_infinite();
}

template<typename T>
LazySeq<T>::LazySeq(const Sequence<T>& seq) {
	generator_ = new SequenceGenerator<T>(seq);
	is_infinite_ = false;
}

template <typename T>
LazySeq<T>::LazySeq() : generator_(nullptr), is_infinite_(false) {
}

template <typename T> 
LazySeq<T>:: ~LazySeq() {
	delete generator_;
}

template <typename T>
void LazySeq<T>::evaluate_up_to(int index) const {
	if (generator_ == nullptr) {
		throw std::invalid_argument("generator cannot be nullptr");
	}
	while (this->cache_.get_size() <= index && generator_->has_next()) {
		this->cache_.append(generator_->get_next());
	}
}

template <typename T>
const Ordinal LazySeq<T>::get_ordinal_length() const {
	if (generator_ == nullptr) {
		return Ordinal(0);
	}
	return generator_->length();
}

template <typename T>
const T& LazySeq<T>::get(int index) const {
	if (index < 0) {
		throw std::out_of_range("Negative index");
	}
	evaluate_up_to(index);

	if (index >= cache_.get_size()) {
		throw std::out_of_range("Index out of range");
	}

	return cache_.get(index);
}

template <typename T>
const T& LazySeq<T>::get_first() const {
	if (!is_infinite_ && cache_.get_size() == 0 && !generator_->has_next()) {
		throw std::out_of_range("lazyseq is empty");
	}
	return get(0);
}	

template <typename T>
const T& LazySeq<T>::get_last() const {
	if (is_infinite_) {
		throw std::logic_error("Cannot get last element of infinite LazySeq");
	}
	while (generator_->has_next()) {
		cache_.append(generator_->get_next());
	}

	if (cache_.get_size() == 0) {
		throw std::out_of_range("LazySequence is empty");
	}

	return cache_.get(cache_.get_size() - 1);
}

template <typename T>
LazySeq<T>* LazySeq<T>::append(const T& item) {
	IGenerator<T>* new_gen = new AppendGenerator<T>(*this, item);
	return new LazySeq<T>(new_gen);
}

template <typename T>
T LazySeq<T>::get(const Ordinal& index) const {
	if (generator_ == nullptr) {
		throw std::out_of_range("LazySeq is empty");
	}

	if (index >= generator_->length()) {
		throw std::out_of_range("Index out of range");
	}

	if (index.is_infinite()) {
		auto* trans_gen = dynamic_cast<ITransfiniteGenerator<T>*>(generator_);
		if (!trans_gen) {
			throw std::logic_error("LazySeq: This sequence is finite and does not support transfinite indexing.");
		}
		return trans_gen->get_by_ordinal_index(index);
	}

	int target_index = index.get_finite_part();
	if (target_index >= cache_.get_size()) {
		evaluate_up_to(target_index);
	}

	return cache_.get(target_index);
}

template <typename T>
LazySeq<T>* LazySeq<T>::map(T(*func)(const T&)) const {
	IGenerator<T>* new_gen = new MapGenerator<T>(*this, func);
	return new LazySeq<T>(new_gen);
}

template <typename T>
LazySeq<T>* LazySeq<T>::where(bool (*predicate)(const T&)) const {
	if (predicate == nullptr) {
		throw std::invalid_argument("where: predicate cannot be nullptr");
	}
	IGenerator<T>* source_clone = this->generator_->clone();
	FilterGenerator<T>* filter_gen = new FilterGenerator<T>(*source_clone, predicate);
	delete source_clone;

	return new LazySeq<T>(filter_gen);
}


template <typename T>
LazySeq<T>* LazySeq<T>::concat(const LazySeq<T>& other) const {
	IGenerator<T>* new_gen = new ConcatGenerator<T>(*this, other);
	return new LazySeq<T>(new_gen);
}

template <typename T>
bool LazySeq<T>:: is_infinite() const {
    return is_infinite_;
}
