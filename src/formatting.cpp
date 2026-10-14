#include <efp/formatting.hpp>
#include <stdexcept>

namespace efp {
    std::string toBinary(const BigInt& value, int width) {
        if (width <= 0) {
            throw std::invalid_argument("Bit width must be positive");
        }

        if (value < 0) {
            throw std::invalid_argument("Cannot format a negative field");
        }

        if (value >= (BigInt(1) << width)) {
            throw std::invalid_argument("Value does not fit the bit width");
        }

        std::string bits(static_cast<std::size_t>(width), '0');
        BigInt remaining = value;

        for (int position = width - 1; position >= 0; --position) {
            bits[static_cast<std::size_t>(position)] =
                (remaining % 2 == 0) ? '0' : '1';

            remaining /= 2;
        }

        return bits;
    }
}