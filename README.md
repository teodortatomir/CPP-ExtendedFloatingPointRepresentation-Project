# Extended Floating Point Representation

A C++ project that converts decimal rational numbers into binary floating-point representations, with support for standard and custom extended formats.

> **Work in progress:** I am rebuilding this project from scratch, applying the new knowledge I have gained since writing the original version. The rebuild focuses on a modular architecture, exact input parsing, correct rounding, and a future browser interface.

The **original implementation is preserved in `main.cpp`**. The new implementation lives in `include/efp/`, `src/`, and `apps/`.

## Current functionality

- Parse signed integers, finite decimals, and repeating decimals:
  - `123`
  - `-12.375`
  - `0.(3)`
  - `12.34(56)`
- Represent inputs exactly as fractions using Boost.Multiprecision integers.
- Simplify fractions using the greatest common divisor.
- Convert to a user-selected floating-point format.
- Apply round-to-nearest, ties-to-even rounding.
- Handle normal numbers, subnormal numbers, positive and negative zero, and overflow to infinity.
- Report whether the conversion is exact.
- Display the sign, exponent, and fraction fields as fixed-width binary strings.
- Reject malformed input.

Exact parsing preserves the original rational value. Conversion to a fixed-size binary format may still require rounding.

## Supported formats

| Format | Total bits | Sign bits | Exponent bits | Fraction bits | Bias |
|---|---:|---:|---:|---:|---:|
| IEEE binary32 | 32 | 1 | 8 | 23 | 127 |
| IEEE binary64 | 64 | 1 | 11 | 52 | 1023 |
| Extended128 | 128 | 1 | 8 | 119 | 127 |
| Extended1024 | 1024 | 1 | 11 | 1012 | 1023 |

Extended128 and Extended1024 are custom formats. Extended128 is not IEEE binary128.

The extended formats increase precision while retaining the exponent ranges of binary32 and binary64, respectively.

## Project structure

```text
include/efp/             Public library headers
src/                    Parsing, conversion, and binary formatting
apps/cli.cpp            Interactive command-line interface
apps/wasm.cpp           Placeholder for the WebAssembly interface
tests/                  Tests being expanded
web/                    Placeholder for the future website
main.cpp                Original implementation
```

The conversion library returns structured results independently of the interface, allowing the CLI and future website to share the same arithmetic.

## Build and run

Requirements:

- A C++17-compatible compiler
- Boost headers

On macOS with Homebrew:

```sh
brew install boost
```

Build the current CLI on Apple Silicon macOS:

```sh
clang++ -std=c++17 -Wall -Wextra -Wpedantic \
  -Iinclude -I/opt/homebrew/include \
  apps/cli.cpp src/parser.cpp src/converter.cpp src/formatting.cpp \
  -o efp_cli
```

Run:

```sh
./efp_cli
```

Choose a format and enter a decimal number. Boost’s include path may differ on other systems.

## Example

For `0.5` using binary32:

```text
Total bits: 32
Sign: 0
Exponent field: 126
Fraction field: 0
Exact: true
Classification: Normal
Bits: 0 | 01111110 | 00000000000000000000000
```

## Tests

Compile and run the current converter tests:

```sh
clang++ -std=c++17 -Wall -Wextra -Wpedantic \
  -Iinclude -I/opt/homebrew/include \
  tests/converter_tests.cpp src/converter.cpp \
  -o converter_tests

./converter_tests
```

Test coverage is still being expanded, particularly for rounding and representation boundaries.

## Next steps

- Expand parser and converter tests.
- Add a reproducible build system.
- Expose the C++ library through WebAssembly.
- Build an interactive website explaining each encoded field.
- Deploy the website through GitHub Pages.

The WebAssembly interface and website are not implemented yet.

## Author

Teodor Tatomir  
[GitHub](https://github.com/teodortatomir) · [LinkedIn](https://www.linkedin.com/in/teodor-tatomir/)