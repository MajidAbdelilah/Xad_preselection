#include "Matrix.hpp"
#include "MatrixView.hpp"
#include "frobenius_norm.hpp"
#include <iostream>
#include <gtest/gtest.h>

TEST(Matrix, Matrix)
{
    Matrix m(10, 4);
    EXPECT_EQ(m.rows(), 10);
    EXPECT_EQ(m.cols(), 4);
    m(0, 0) = 1.4;
    EXPECT_EQ(m(0, 0), 1.4);
    // EXPECT_EQ(m(1, 1), 0); // undefined behaviour – can be anything
    // m(11, 0); // undefined behaviour – no bounds checks in release mode
    const auto& m2(m);
    EXPECT_EQ(m2(0, 0), 1.4);
    Matrix m3(m);
    EXPECT_EQ(m3(0, 0), 1.4);
    m3(0, 0) = 42.0;
    EXPECT_EQ(m3(0, 0), 42.0);
    EXPECT_EQ(m(0, 0), 1.4);
    Matrix m4(std::move(m3));
    EXPECT_EQ(m4(0, 0), 42);
    m4 = m;
    EXPECT_EQ(m4(0, 0), 1.4);
    m4(0, 0) = 3.0;
    EXPECT_EQ(m4(0, 0), 3.0);
    EXPECT_EQ(m(0, 0), 1.4);
    Matrix m5(2, 3);
    m5 = std::move(m4);
    EXPECT_EQ(m5(0, 0), 3);
    // EXPECT_EQ(m4(0, 0), 0); // undefined behaviour – should never happen
    Matrix m7(2, 3);
    m7 = std::move(m4);
    Matrix m6(2, 3, 42.0);
    EXPECT_EQ(m6(1, 1), 42);
}

Matrix create_Matrix_from_view(MatrixView mv)
{
    return mv;
}

TEST(MatrixView, MatrixView)
{
    Matrix m(10, 4);
    m(2, 3) = 1.4;
    MatrixView v(m, 1, 2, 2, 2); // start_row, start_col, num_rows, num_cols
    EXPECT_EQ(v(1, 1), 1.4);
    EXPECT_EQ(v.rows(), 2);
    EXPECT_EQ(v.cols(), 2);
    v(1, 0) = 42.2;
    EXPECT_EQ(m(2, 2), 42.2);
    Matrix m2 = v;
    EXPECT_EQ(m2.rows(), 2);
    EXPECT_EQ(m2.cols(), 2);
    EXPECT_EQ(m2(1, 1), 1.4);
    EXPECT_EQ(m2(1, 0), 42.2);
    auto p_m3 = std::make_unique<Matrix>(m); // dynamically-allocated copy of m
    MatrixView v3(*p_m3, 1, 2, 2, 2); // create a view
    p_m3 = nullptr; // destroy p_m3
    EXPECT_EQ(v3(1, 1), 1.4);
    Matrix m3 = create_Matrix_from_view(v3);
    EXPECT_EQ(m3(1, 1), 1.4);
}

TEST(FrobeniusNorm, FrobeniusNorm)
{
    Matrix m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    EXPECT_NEAR(frobenius_norm(m), frobenius_norm_baseline_unoptimized(m), 1e-6);
    Matrix m2(2, 2, 1);
    EXPECT_NEAR(frobenius_norm(m2), frobenius_norm_baseline_unoptimized(m2), 1e-6);
    Matrix m3(2, 2, 0);
    EXPECT_NEAR(frobenius_norm(m3), frobenius_norm_baseline_unoptimized(m3), 1e-6);
    Matrix m4(2, 2, -1);
    EXPECT_NEAR(frobenius_norm(m4), frobenius_norm_baseline_unoptimized(m4), 1e-6);
    Matrix M5(10, 10, 2);
    MatrixView v(M5, 1, 1, 3, 4);
    EXPECT_NEAR(frobenius_norm(v), frobenius_norm_baseline_unoptimized(v), 1e-6);
    Matrix M6(10, 10, 1234.1234);
    MatrixView v2(M6, 2, 5, 6, 4);
    EXPECT_NEAR(frobenius_norm(v2), frobenius_norm_baseline_unoptimized(v2), 1e-6);
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}