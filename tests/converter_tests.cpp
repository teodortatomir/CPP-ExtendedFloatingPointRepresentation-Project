#include <iostream>
#include <cassert>
#include <efp/converter.hpp>

int main() {
    auto result = efp::convert(
        efp::Rational{false, 1, 2},
        efp::binary32
    );

    assert(result.classification == efp::Classification::Normal);
    assert(result.exponentField == 126);
    assert(result.fractionField == 0);
    assert(result.exact);

    std::cout << "All converter tests passed!\n";
    return 0;
}