#pragma once

#include <string_view>
#include <efp/rational.hpp>

namespace efp {
    Rational parseDecimal(std::string_view input);
}