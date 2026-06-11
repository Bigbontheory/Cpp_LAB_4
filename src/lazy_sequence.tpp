

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
LazySeq<T>::LazySeq(const T* items, int count) {
	for (int i = 0; i < count; ++i) {
		cache_.append(items[i]);
	}
	is_infinite_ = false;
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
LazySeq<T> ::LazySeq() : generator_(nullptr) {}

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
IEnumerator<T>* LazySeq<T>::get_enumerator() const {
	return new LazyEnumerator(this);
}

template <typename T>
LazySeq<T>* LazySeq<T>::append(const T& item) {
	AppendGenerator<T>* temp_gen = new AppendGenerator<T>(item, generator_);
	return new LazySeq<T>(temp_gen);
}

template <typename T>
const T& LazySeq<T>::get(const Ordinal& index) const {
	if (index.is_infinite()) {
		return generator_->get_by_ordinal(index);
	}

	int target_index = index.get_finite_part();

	if (target_index < cache_.get_size()) {
		return cache_.get(target_index);
	}

	evaluate_up_to(target_index);
	return cache_.get(target_index);
}

template <typename T>
LazySeq<T>* LazySeq<T>::prepend(const T& item) {
	PrependGenerator<T>* temp_gen = new PrependGenerator<T> (item, generator_);
    return new LazySeq<T>(temp_gen);
}

template <typename T>
LazySeq<T>* LazySeq<T>::map(T(*mapper)(const T& elem)) const {
	MapGenerator<T>* map_gen = new MapGenerator<T>(this->generator_, mapper);
	return new LazySeq<T>(map_gen);
}

template <typename T>
LazySeq<T>* LazySeq<T>::where(bool (*predicate)(const T& elem)) const {
	FilterGenerator<T>* filter_gen = new FilterGenerator<T>(this->generator_, predicate);
	return new LazySeq<T>(filter_gen);
}


template <typename T>
LazySeq<T>* LazySeq<T>::concat(const LazySeq<T>& other) const {
	ConcatGenerator<T>* new_gen = new ConcatGenerator<T>(
		this->generator_->clone(),
		other.generator_->clone()
	); 

	return new LazySeq<T>(new_gen);
}

template <typename T>
bool LazySeq<T>:: is_infinite() const {
    return is_infinite_;
}

template <typename T>
LazySeq<T>* LazySeq<T>::insert_at_ordinal(const T& element, Ordinal pos) {
	InsertElementGenerator<T>* gen = new InsertElementGenerator<T>(
		this->generator_->clone(),
		element,
		pos
	);
	return new LazySeq<T>(gen);
}