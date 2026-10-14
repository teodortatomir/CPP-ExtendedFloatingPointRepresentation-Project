#include <efp/parser.hpp>
#include <efp/converter.hpp>
#include <efp/formatting.hpp>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    try {
        std::cout << "Choose a format:\n"
                  << "1. Binary32\n"
                  << "2. Binary64\n"
                  << "3. Extended128\n"
                  << "4. Extended1024\n"
                  << "Selection: ";

        std::string selection;
        if (!std::getline(std::cin, selection)) {
            throw std::runtime_error("Could not read format selection");
        }

        efp::Format format;

        if (selection == "1") {
            format = efp::binary32;
        } else if (selection == "2") {
            format = efp::binary64;
        } else if (selection == "3") {
            format = efp::extended128;
        } else if (selection == "4") {
            format = efp::extended1024;
        } else {
            throw std::invalid_argument("Invalid format selection");
        }

        std::cout << "\nEnter a decimal number "
                  << "(for example, -12.34(56)): ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            throw std::runtime_error("Could not read number");
        }

        efp::Rational value = efp::parseDecimal(input);
        efp::ConversionResult result = efp::convert(value, format);

        std::cout << "\nTotal bits: "
                  << 1 + format.exponentBits + format.fractionBits
                  << '\n';

        std::cout << "Sign: "
                  << static_cast<int>(result.negative) << '\n';
        std::cout << "Exponent field: " << result.exponentField << '\n';
        std::cout << "Fraction field: " << result.fractionField << '\n';
        std::cout << "Exact: " << std::boolalpha << result.exact << '\n';

        std::cout << "Classification: ";
        switch (result.classification) {
            case efp::Classification::Zero:
                std::cout << "Zero";
                break;
            case efp::Classification::Normal:
                std::cout << "Normal";
                break;
            case efp::Classification::Subnormal:
                std::cout << "Subnormal";
                break;
            case efp::Classification::Infinity:
                std::cout << "Infinity";
                break;
        }
        std::cout << '\n';

        std::cout << "Bits: "
                  << static_cast<int>(result.negative) << " | "
                  << efp::toBinary(
                         result.exponentField, format.exponentBits)
                  << " | "
                  << efp::toBinary(
                         result.fractionField, format.fractionBits)
                  << '\n';

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}