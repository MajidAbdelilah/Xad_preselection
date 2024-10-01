#include "MatrixView.hpp"
#include <iostream>

void f(const Matrix& m) {
    Matrix m2 = m; // normal copy, no matter if m was actually a view or not
}

int main()
{

    Matrix m(10, 4);
    m(2, 3) = 1.4;
    MatrixView v(m, 1, 2, 2, 2); // start_row, start_col, num_rows, num_cols
    std::cout << v(1, 1) << "\n"; // prints 1.4
    std::cout << "(" << v.rows() << ", " << v.cols() << ")\n"; // prints (2, 2)
    v(1, 0) = 42.2;
    std::cout << m(2, 2) << "\n"; // prints 42
    Matrix m2 = v;
    std::cout << "rc: " << m2.rows() << ", " << m2.cols() << "\n";
    std::cout << m2(1, 1) << ", " << m2(1, 0) << "\n"; // prints 1.4, 42.2
    std::cout << "(" << m2.rows() << ", " << m2.cols() << ")\n"; // prints (2, 2)
    auto p_m3 = std::make_unique<Matrix>(m); // dynamically-allocated copy of m
    MatrixView v3(*p_m3, 1, 2, 2, 2); // create a view
    p_m3 = nullptr; // destroy p_m3
    std::cout << v3(1, 1) << "\n"; // still prints 1.4 – data preserved in the view

    f(m); // regular Matrix passed - valid
    f(v3); // a view passed by reference - valid
}