#include <efp/parser.hpp>
#include <stdexcept>

namespace efp {
    namespace {
        BigInt gcd(BigInt a, BigInt b) { // We can use gcd to simplify the fraction that we are computing
            while(b > 0) {
                BigInt r = a % b;
                a = b;
                b = r;
            }

            return a;
        }
    }

    Rational parseDecimal(std::string_view input) {
        if(input.empty())
            throw std::invalid_argument("Input cannot be empty");
        
        Rational result;
        std::size_t position = 0;

        if(input[position] == '-' || input[position] == '+') {
            if(input[position] == '-') {
                result.negative = true;
                position++;
            } else if(input[position] == '+') {
                position++;
            }

            if(position == input.size()) 
                throw std::invalid_argument("Expected digits after sign");
        }
        
        
        bool seenDecimalPoint = false;
        bool seenDigit = false;

        BigInt repeatingValue = 0; // We need to keep the values inside the () (period i think, in romanian that's how it's called) separate
        BigInt repeatingScale = 1; // Power of 10 for period
        bool seenRepeatingDigit = false;

        while(position < input.size()) {
            if(input[position] >= '0' && input[position] <= '9') {
                result.numerator = result.numerator * 10 + (input[position] - '0');
                if(seenDecimalPoint) { // Then we should also compute the denominator
                    result.denominator *= 10; // One more power of 10 for the denominator then
                }
                seenDigit = true; // Kinda redundant but it's ok:)
            } else if(input[position] == '.') {
                if(seenDecimalPoint) { // Then we have at least two decimal points, so we should throw an exception
                    throw std::invalid_argument("We can't have two decimal points");
                } else {
                    seenDecimalPoint = true; // This is the first one
                }
            } else if(input[position] == '(') {
                if(!seenDecimalPoint || !seenDigit) { // You cannot have a period out of nowhere
                    throw std::invalid_argument("Expected decimal digits before repeating part");
                }
                
                position++;
                while(position < input.size() && input[position] != ')') { // treat the period separately
                    if(input[position] >= '0' && input[position] <= '9') {
                        repeatingValue = repeatingValue * 10 + (input[position] - '0');
                        repeatingScale = repeatingScale * 10;
                        seenRepeatingDigit = true; // Again a bit redundant, i don't really know if an if statement on the seenRepeatingDigit would help since it would be a new branch for the CPU pipeline, i'll look into it
                    } else {
                        throw std::invalid_argument("Expected a decimal digit");
                    }

                    position++;
                }

                if(!seenRepeatingDigit) {
                    throw std::invalid_argument("Repeating part cannot be empty");
                }  

                if(position == input.size() || input[position] != ')') {
                    throw std::invalid_argument("Expected the open period to have a closing bracket also");
                } else if(input[position] == ')' && position != input.size() - 1) {
                    throw std::invalid_argument("You cannot have digits after the period!");
                }

                // Now we should build the numerator / denominator using the period :
                // The formula states :
                // Let A = all digits before the repeating part, ignoring the decimal point
                // Let m = number of non-repeating fractional digits
                // Let R = integer represented by the repeating digits
                // Let r = number of repeating digits
                
                // Then, the number magnitude can be calculated using this ugly formula:
                // x = (A * (10 ^ r - 1) + R) / (10 ^ m * (10 ^ r - 1))
                // To be honest, i have just learned about it but it makes sense, it's elementary maths if you break it in pieces and try to understand it

                result.numerator = result.numerator * (repeatingScale - 1) + repeatingValue;
                result.denominator *= (repeatingScale - 1);
                break; // Since we went past the period, we can break here
            }
            else {
                throw std::invalid_argument("Expected a decimal digit");
            }
                
            position++;
        }

        if(!seenDigit)
            throw std::invalid_argument("Expected at least one digit");
        
        // Since we have implemented gcd, now we can simplify the fraction by deviding both numerator and denominator by gcd of them 

        BigInt gcdOfEquation = gcd(result.numerator, result.denominator);

        result.numerator /= gcdOfEquation;
        result.denominator /= gcdOfEquation;

        return result;
    }
};