#include <efp/converter.hpp>
#include <stdexcept>

namespace efp {
    ConversionResult convert(const Rational& value, const Format& format) {
        if(value.numerator < 0 || value.denominator <= 0)
            throw std::invalid_argument("Expected valid value");

        if((format.exponentBits < 2 || format.exponentBits > 15)
        || (format.fractionBits < 1 || format.fractionBits > 4096))
            throw std::invalid_argument("Invalid format bit widths");
        
        int expectedBias = (1 << (format.exponentBits - 1)) - 1;

        if(format.bias != expectedBias) {
            throw std::invalid_argument("Invalid exponent bias");
        }
        
        ConversionResult result;
        result.negative = value.negative;

        if(value.numerator == 0) {
            return result;
        }

        // Now we need to normalize
        // For a magnitude x = N/D
        // Then, we need to find 1 <= x / (2 ^ e) < 2

        BigInt scaledNumerator = value.numerator;
        BigInt scaledDenominator = value.denominator;
        int exponent = 0;

        while(scaledNumerator >= 2 * scaledDenominator) {
            scaledDenominator *= 2;
            exponent++;
        }

        while(scaledNumerator < scaledDenominator) {
            scaledNumerator *= 2;
            exponent--;
        }

        // Now let's define the exponent range, with k exponent bits and bias B we will have
        // minExponent = 1 - B
        // maxExponent = (2 ^ k - 2) - B
        // This is because we reserve exponent fields 0 and 2^k - 1 for special values

        int minExponent = 1 - format.bias;
        int maxExponent = ((1 << format.exponentBits) - 2) - format.bias;

        if(exponent > maxExponent) { // Overflow
            result.classification = Classification::Infinity;
            result.exponentField = (BigInt(1) << format.exponentBits) - 1;
            result.exact = false;

            return result;
        }

        if(exponent >= minExponent) {
            BigInt unit = BigInt(1) << format.fractionBits;
            BigInt scaled = scaledNumerator * unit;
            BigInt significand = scaled / scaledDenominator;
            BigInt reminder = scaled % scaledDenominator;

            result.exact = (reminder == 0);

            BigInt twiceReminder = reminder * 2;

            // Above half: round up
            // Below half: keep value
            // Exactly half: choose the even integer
            if(twiceReminder > scaledDenominator 
                || (twiceReminder == scaledDenominator && significand % 2 != 0)) {
                significand++;
            }

            if(significand == 2 * unit) {
                significand /= 2;
                exponent++;
            }

            // We need to check overflow again
            if(exponent > maxExponent) {
                result.classification = Classification::Infinity;
                result.exponentField = (BigInt(1) << format.exponentBits) - 1;
                result.exact = false;

                return result;
            }

            result.classification = Classification::Normal;
            result.exponentField = exponent + format.bias;
            result.fractionField = significand - unit; // remove the one
            
            return result;
        } else { // Treat the subnormal numbers
            BigInt unit = BigInt(1) << format.fractionBits;
            int shift = format.fractionBits - minExponent;

            BigInt scaled = value.numerator << shift;
            BigInt fraction = scaled / value.denominator;
            BigInt remainder = scaled % value.denominator;
            result.exact = (remainder == 0);

            BigInt twiceRemainder = remainder * 2;

            if(twiceRemainder > value.denominator 
                || (twiceRemainder == value.denominator && fraction % 2 != 0)) {
                fraction++;
            }

            if(fraction == 0) {
                result.classification = Classification::Zero;
                result.exponentField = 0;
                result.fractionField = 0;
            } else if(fraction == unit) {
                result.classification = Classification::Normal;
                result.exponentField = 1;
                result.fractionField = 0;
            } else {
                result.classification = Classification::Subnormal;
                result.exponentField = 0;
                result.fractionField = fraction;
            }

            return result;
        }
        throw std::logic_error("Nonzero conversion is not implemented yet");
    }
}