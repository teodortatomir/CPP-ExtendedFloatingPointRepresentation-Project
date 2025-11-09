# Extended Floating Point Representation without losing precision

This project is a C++ utility designed to address the inherent precision limitations of the IEEE 754 standard (used in 32-bit float and 64-bit double). 
It implements a custom, arbitrary-precision system to convert decimal rational numbers into one of two significantly extended binary formats.
The primary achievement is demonstrating the structure and necessity of extended precision by increasing the mantissa bits far beyond standard hardware capacity.

Implemented using division on arrays, it can support numbers between [-10^310, 10^310]!

It supports numbers with period digits like : -123.4(56)

Used C++ Standard Library.

You need a C++ compiler to execute it!

*  Author : Teodor Tatomir
*  LinkedIn : www.linkedin.com/in/teodor-tatomir
*  GitHub : @teodortatomir
