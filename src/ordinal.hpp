#pragma once 

#include <cstddef>
#include <stdexcept>

class Ordinal {
private:
	std::size_t omega_count;
	std::size_t finite_part;

public:
	Ordinal() : omega_count(0), finite_part(0) {}
	explicit Ordinal(std::size_t finite) : omega_count(0), finite_part(finite) {}

	Ordinal(std::size_t omega, std::size_t finite) : omega_count(omega), finite_part(finite) {

	}

	static Ordinal omega()
	{
		return Ordinal(1, 0);
	}

	bool is_infinite() const { return omega_count > 0; }

	std::size_t get_omega_count() const {
		return omega_count;
	}

	std::size_t get_finite_part() const {
		return finite_part;
	}

	std::size_t get_value() const {
		if (is_infinite())
		{
			throw std::logic_error("cannot get integer value in infinite ordinal");
		}
		return finite_part;
	}
	
	friend bool operator==(const Ordinal& lhs, const Ordinal& rhs) {
		return lhs.omega_count == rhs.omega_count && lhs.finite_part == rhs.finite_part;
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

	bool operator>=(const Ordinal& other) const {
		return *this > other || *this == other;
	}


	Ordinal operator+(const Ordinal& other) const {
		if (other.omega_count == 0) {
			return Ordinal(this->omega_count, this->finite_part + other.finite_part);
		}
		return Ordinal(this->omega_count + other.omega_count, other.finite_part);
	}

	Ordinal& operator--() {
		if (finite_part > 0) {
			finite_part--;
			return *this;
		}
		if (omega_count > 0) {
			throw std::logic_error("Ordinal decrement: cannot decrement at a limit point (omega border).");
		}

		throw std::logic_error("Ordinal decrement: cannot decrement zero.");
	}
};
