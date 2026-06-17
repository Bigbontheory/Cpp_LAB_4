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
LazySeq<T>::LazySeq(const LazySeq<T>& other)
	: is_infinite_(other.is_infinite_)
{
	this->cache_ = MutableArraySequence<T>(other.cache_);

	if (other.generator_ != nullptr) {
		this->generator_ = other.generator_->clone();
	}
	else {
		this->generator_ = nullptr;
	}
}

template <typename T>
LazySeq<T>::LazySeq(T* items, int count) : is_infinite_(false) {
	this->cache_ = MutableArraySequence<T>(items, count);
	generator_ = new SequenceGenerator<T>(this->cache_);
}

template <typename T>
LazySeq<T>::LazySeq(const IGenerator<T>* gen)
	: generator_(gen->clone()), is_infinite_(generator_->length().is_infinite()) {
}

template <typename T>
LazySeq<T>* LazySeq<T>::clone() const {
	return new LazySeq<T>(*this);
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
LazySeq<T>::LazySeq(const MutableArraySequence<T>& seq) {
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
	if (cache_.get_size() > index) return;

	while (cache_.get_size() <= index) {

		if (!generator_->has_next()) {
			throw std::out_of_range("Generator exhausted before reaching index");
		}

		T val = generator_->get_next();
		cache_.append(val);
	}
}

template <typename T>
const Ordinal LazySeq<T>::get_ordinal_length() const {
	if (generator_ == nullptr) {
		return Ordinal(0, 0);
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
	Ordinal len = get_ordinal_length();
	if (len.get_omega_count() == 0 && len.get_finite_part() == 0) {
		throw std::out_of_range("LazySequence is empty");
	}
	if (len.get_omega_count() > 0 && len.get_finite_part() == 0) {
		throw std::logic_error("LazySequence ends at a limit ordinal (e.g., w): it has no last element.");
	}

	Ordinal last_ordinal_index = len;
	--last_ordinal_index;


	return this->get(last_ordinal_index);
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
		return generator_->get_by_ordinal(index);
	}

	std::size_t target_index = index.get_finite_part();
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
	IGenerator<T>* new_gen = new ConcatGenerator<T>(this->get_generator(), other.get_generator());
	return new LazySeq<T>(new_gen);
}

template <typename T>
bool LazySeq<T>:: is_infinite() const {
    return is_infinite_;
}

template <typename T>
LazySeq<T>* LazySeq<T>::insert_at(const T& element, const Ordinal& index) const {
	if (generator_ == nullptr) {
		throw std::logic_error("insert_at: generator cannot be nullptr");
	}

	InsertAtGenerator<T>* insert_gen = new InsertAtGenerator<T>(this->generator_, element, index);
	return new LazySeq<T>(insert_gen);
}

template <typename T>
LazySeq<T>* LazySeq<T>::insert_at(const T& element, int index) {
	if (index < 0) {
		throw std::out_of_range("Index cannot be negative");
	}

	InsertAtGenerator<T>* insert_gen = new InsertAtGenerator<T>(this->generator_, element, Ordinal(index));
	return new LazySeq<T>(insert_gen);
}

template <typename T>
LazySeq<T>* LazySeq<T>::prepend (const T& element) {
	if (generator_ == nullptr) {
		throw std::logic_error("prepend: generator cannot be nullptr");
	}

	PrependGenerator<T>* prepend_gen = new PrependGenerator<T>(this->generator_, element);
	return new LazySeq<T>(prepend_gen);
}

template <typename T>
LazySeq<T>* LazySeq<T>::insert_at(const Ordinal& index, const LazySeq<T>& other) const {

	if (index > this->get_ordinal_length()) {
		throw std::out_of_range("LazySeq::insert_at: index out of bounds");
	}

	InsertSeqAtGenerator<T>* new_gen = new InsertSeqAtGenerator<T>(
		this->generator_,
		other.generator_,
		index
	);

	return new LazySeq<T>(new_gen);
}

template <typename T>
LazySeq<T>* LazySeq<T>::replace_at(const T& element, const Ordinal& index) const {
	ReplaceAtGenerator<T> replacer(this->generator_, element, index);
	return new LazySeq<T>(&replacer);
}
