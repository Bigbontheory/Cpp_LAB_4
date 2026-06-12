#pragma once

#include "ordinal.hpp"

template<typename T>
class IGenerator {
public:
	virtual T get_next() = 0;
	virtual bool has_next() const = 0;
	virtual Ordinal length() const = 0;
	virtual IGenerator<T>* clone() const = 0;
};

