#pragma once 

#include <cstddef>
#include <stdexcept>

class Ordinal {
private:
	std::size_t omega_count;
	std::size_t finite_part;

public:
	Ordinal() : omega_count(0), finite_part(0) {}
	explicit Ordinal(std::size_t finite) : omega_count(0), finite_part(finite) {
		if (finite < 0) throw std::invalid_argument("Ordinal parts cannot be negative");
	}

	Ordinal(std::size_t omega, std::size_t finite) : omega_count(omega), finite_part(finite) {
		if (omega < 0 || finite < 0) throw std::invalid_argument("Ordinal parts cannot be negative");
	}

	static Ordinal omega()
	{
		return Ordinal(1, 0);
	}

	bool is_finite() const { return omega_count == 0; }

	std::size_t get_omega_count() const {
		return omega_count;
	}

	std::size_t get_finite_part() const {
		return finite_part;
	}

	bool operator==(const Ordinal& other) const {
		return omega_count == other.omega_count && finite_part == other.finite_part;
	}

	bool operator!=(const Ordinal& other) const {
		return !(*this == other);
	}

	bool operator<(const Ordinal& other) const {
		if (omega_count == other.omega_count) {
			return finite_part < other.finite_part;
		}
		return omega_count < other.omega_count;
	}

	bool operator<=(const Ordinal& other) const {
		return *this < other || *this == other;
	}

	bool operator>(const Ordinal& other) const {
		return other < *this;
	}

	Ordinal operator+(const Ordinal& other) const {
		if (other.omega_count > 0) {
			return Ordinal(this->omega_count + other.omega_count, other.finite_part);
		}
		else {
			return Ordinal(this->omega_count, this->finite_part + other.finite_part);
		}
	}
};