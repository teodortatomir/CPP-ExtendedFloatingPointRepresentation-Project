#pragma once

#include <efp/rational.hpp>
#include <string>

namespace efp {
    std::string toBinary(const BigInt& value, int width);
}