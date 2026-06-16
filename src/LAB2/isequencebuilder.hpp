#pragma once

template<typename T>
class Sequence_LAB_2;

template <typename T>
class ISequenceBuilder {
public:
	virtual ~ISequenceBuilder() = default;
	virtual void append(const T & item) = 0;
	virtual Sequence_LAB_2<T>* build() = 0;
};

