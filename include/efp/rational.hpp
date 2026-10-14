#pragma once

#include <boost/multiprecision/cpp_int.hpp>

namespace efp {
    using BigInt = boost::multiprecision::cpp_int;

    struct Rational {
        bool negative = false;
        BigInt numerator = 0; // non-negative
        BigInt denominator = 1; // strictly positive
    };
};
