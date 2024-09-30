#include "Matrix.hpp"
#include <iostream>

int main()
{
    Matrix m(10, 4);
    std::cout << "(" << m.rows() << ", " << m.cols() << ")\n"; // prints (10, 4)
    m(0, 0) = 1.4;
    std::cout << m(0, 0) << "\n"; // prints 1.4
    std::cout << m(1, 1) << "\n"; // undefined behaviour – can be anything
    m(11, 0); // undefined behaviour – no bounds checks in release mode
    const auto& m2(m);
    std::cout << m2(0, 0) << "\n"; // prints 1.4
    Matrix m3(m);
    std::cout << m3(0, 0) << "\n"; // prints 1.4
    m3(0, 0) = 42.0;
    std::cout << m3(0, 0) << ", " << m(0, 0) << "\n"; // prints 42.0, 1.4
    Matrix m4(std::move(m3)); // no data is copied, using original m3 is undefined
    std::cout << m4(0, 0) << "\n"; // prints 42
    m4 = m;
    std::cout << m4(0, 0) << "\n"; // prints 1.4
    m4(0, 0) = 3.0;
    std::cout << m4(0, 0) << ", " << m(0, 0) << "\n"; // prints 3.0, 1.4
    Matrix m5(2, 3);
    m5 = std::move(m4); // move assign – no copies made
    std::cout << m5(0, 0) << "\n"; // prints 3
    // std::cout << m4(0, 0) << "\n"; // undefined behaviour – should never happen
    Matrix m7(2, 3);
    m7 = std::move(m4); // move assign – no copies made
    Matrix m6(2, 3, 42.0); // construct with initialiser value
    std::cout << m6(1, 1) << "\n"; // prints 42

}