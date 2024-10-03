#include "Matrix.hpp"
#include <iostream>

Matrix::Matrix(const long rows, const long cols)
    : _rows(rows), _cols(cols), data(new double[rows * cols]), data_ref_count(new long(1))
{
    *data_ref_count = 1;
}

Matrix::Matrix(const long rows, const long cols, const double val)
    : _rows(rows), _cols(cols), data(new double[rows * cols]), data_ref_count(new long(1))
{
    *data_ref_count = 1;
    for (long i = 0; i < rows * cols; i++)
    {
        data[i] = val;
    }
}

Matrix::Matrix(const Matrix &m)
    : _rows(m._rows), _cols(m._cols), data(new double[m.rows() * m.cols()]), data_ref_count(new long(1))
{
    (*data_ref_count) = 1;
    for (long i = 0; i < m.rows() * m.cols(); i++)
    {
        data[i] = m.data[i];
    }
}

Matrix::Matrix(Matrix &&m)
    : _rows(m._rows), _cols(m._cols), data(m.data), data_ref_count(m.data_ref_count)
{
    (*data_ref_count)++;
}

Matrix::~Matrix()
{
    // std::cout << "Destructor called" << std::endl;
    if (--(*data_ref_count) == 0)
    {
        delete[] data;
        delete data_ref_count;
    }
}

Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m)
    {
        return *this;
    }
    if (--(*data_ref_count) == 0)
    {
        delete[] data;
        delete data_ref_count;
    }
    _rows = m._rows;
    _cols = m._cols;
    data = new double[m.rows() * m.cols()];
    data_ref_count = new long(1);
    (*data_ref_count) = 1;
    for (long i = 0; i < m.rows() * m.cols(); i++)
    {
        data[i] = m.data[i];
    }
    return *this;
}

Matrix &Matrix::operator=(Matrix &&m)
{
    if (this == &m)
    {
        return *this;
    }
    if (--(*data_ref_count) == 0)
    {
        delete[] data;
        delete data_ref_count;
    }
    _rows = m._rows;
    _cols = m._cols;
    data = m.data;
    data_ref_count = m.data_ref_count;

    (*data_ref_count)++;
    return *this;
}

double &Matrix::operator()(const long i, const long j)
{
    return data[i * _cols + j];
}

double Matrix::operator()(const long i, const long j) const
{
    return data[i * _cols + j];
}

long Matrix::rows() const
{
    return _rows;
}

long Matrix::cols() const
{
    return _cols;
}

void Matrix::print() const
{
    for (long i = 0; i < _rows; i++)
    {
        for (long j = 0; j < _cols; j++)
        {
            std::cout << data[i * _cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

double *Matrix::get_data() const
{
    return data;
}


long *Matrix::get_data_ref_count() const
{
    return data_ref_count;
}