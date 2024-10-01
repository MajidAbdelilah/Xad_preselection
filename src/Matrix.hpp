#pragma once
#include <vector>

class MatrixView;

class Matrix
{
public:
    Matrix(const long rows, const long cols);
    Matrix(const long rows, const long cols, const double val);
    Matrix(const Matrix &m);
    Matrix(Matrix &&m);
    ~Matrix();
    Matrix &operator=(const Matrix &m);
    Matrix &operator=(Matrix &&m);
    double &operator()(const long i, const long j);
    double operator()(const long i, const long j) const;
    long rows() const;
    long cols() const;
    void print() const;
    double *get_data() const;
    long *get_data_ref_count() const;
private:
    long _rows;
    long _cols;
    double *data;
    long *data_ref_count;
};