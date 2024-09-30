#include "Matrix.hpp"
#include <iostream>

Matrix::Matrix(const long _rows, const long _cols)
    : _rows(_rows), _rows_start(0), _cols(_cols), _cols_start(0), reported_colomns(_cols), data(new double[_rows * _cols])
{

}

Matrix::Matrix(const long _rows, const long _cols, const double val)
    : _rows(_rows), _rows_start(0), _cols(_cols), _cols_start(0), reported_colomns(_cols), data(new double[_rows * _cols])
{
    for (long i = 0; i < _rows * _cols; i++)
    {
        data[i] = val;
    }
}

Matrix::Matrix(const Matrix &m)
    : _rows(m._rows), _cols(m._cols), _rows_start(m._rows_start), _cols_start(m._cols_start), reported_colomns(m.reported_colomns), data(new double[m._rows * m._cols])
{
    for (long i = 0; i < m._rows * m._cols; i++)
    {
        data[i] = m.data[i];
    }
}

Matrix::Matrix(Matrix &&m)
    : _rows(m._rows), _cols(m._cols), _rows_start(m._rows_start), _cols_start(m._cols_start), reported_colomns(m.reported_colomns), data(m.data)
{
    m._rows = 0;
    m._cols = 0;
    m.reported_colomns = 0;
    m._rows_start = 0;
    m._cols_start = 0;
    m.data = nullptr;
}

Matrix::~Matrix()
{
    delete[] data;
}

Matrix &Matrix::operator=(const Matrix &m)
{
    if (this != &m)
    {
        delete[] data;
        _rows = m._rows;
        _rows_start = m._rows_start;
        _cols = m._cols;
        _cols_start = m._cols_start;
        reported_colomns = m.reported_colomns;
        data = new double[m._rows * m._cols];
        for (long i = 0; i < m._rows * m._cols; i++)
        {
            data[i] = m.data[i];
        }
    }
    return *this;
}

Matrix &Matrix::operator=(Matrix &&m)
{
    if (this != &m)
    {
        delete[] data;
        _rows = m._rows;
        _rows_start = m._rows_start;
        _cols = m._cols;
        _cols_start = m._cols_start;
        reported_colomns = m.reported_colomns;
        data = m.data;
        m._rows = 0;
        m._rows_start = 0;
        m._cols = 0;
        m._cols_start = 0;
        m.reported_colomns = 0;
        m.data = nullptr;
    }
    return *this;
}

double &Matrix::operator()(const long i, const long j)
{
    return data[(_rows_start + i) * _cols + (_cols_start + j)];
}

double Matrix::operator()(const long i, const long j) const
{
    return data[(_rows_start + i) * _cols + (_cols_start + j)];
}

long Matrix::rows() const
{
    return _rows;
}

long Matrix::cols() const
{
    return reported_colomns;
}

void Matrix::print() const
{
    for (long i = 0; i < _rows; i++)
    {
        for (long j = 0; j < reported_colomns; j++)
        {
            std::cout << data[(_rows_start + i) * _cols + (_cols_start + j)] << " ";
        }
        std::cout << std::endl;
    }
}

