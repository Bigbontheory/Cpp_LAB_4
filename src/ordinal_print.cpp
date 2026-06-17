#include "ordinal_print.hpp"

std::ostream& operator<<(std::ostream& os, const Ordinal& ord) {
    if (!ord.is_infinite()) {
        return os << ord.get_finite_part();
    }

    if (ord.get_omega_count() > 1) {
        os << "w*" << ord.get_omega_count();
    }
    else {
        os << "w";
    }

    if (ord.get_finite_part() > 0) {
        os << " + " << ord.get_finite_part();
    }

    return os;
}