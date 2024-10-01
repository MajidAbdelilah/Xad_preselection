#pragma once

#include "Matrix.hpp"

class MatrixView
{
public:
    MatrixView(Matrix &m, const long start_row, const long start_col, const long num_rows, const long num_cols);
    MatrixView(const MatrixView &m);
    MatrixView(MatrixView &&m);
    ~MatrixView();
    MatrixView &operator=(const MatrixView &m);
    MatrixView &operator=(MatrixView &&m);
    double &operator()(const long i, const long j);
    double operator()(const long i, const long j) const;
    long rows() const;
    long cols() const;
    void print() const;
    double *get_data() const;
    long *get_data_ref_count() const;
    operator Matrix() const;
private:
    long _rows;
    long _rows_start;
    long _cols;
    long _cols_start;
    long _reported_cols;
    double *data;
    long *data_ref_count;
};