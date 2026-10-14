#pragma once

#include <efp/format.hpp>
#include <efp/rational.hpp>

namespace efp {
    enum class Classification {
        Zero,
        Normal,
        Subnormal,
        Infinity
    };

    struct ConversionResult {
        bool negative = false;
        BigInt exponentField = 0;
        BigInt fractionField = 0;
        Classification classification = Classification::Zero;
        bool exact = true;
    };

    ConversionResult convert(const Rational& value, const Format& format);
}