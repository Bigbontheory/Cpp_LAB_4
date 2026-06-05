#pragma once

#include "Ordinal.hpp"
#include <iostream>

 inline std::ostream& operator<<(std::ostream& os, const Ordinal& ord) {
	if (ord.get_omega_count() > 0) {
		if (ord.get_omega_count() > 1) {
			os << "w*" << ord.get_omega_count();
		}
		else {
			os << "w";
		}
		if (ord.get_finite_part() > 0) {
			os << " + " << ord.get_finite_part();
		}
	}
	else {
		os << ord.get_finite_part();
	}
	return os;
}