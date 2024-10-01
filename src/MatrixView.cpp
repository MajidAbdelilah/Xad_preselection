#include "MatrixView.hpp"
#include <stdexcept>
#include <iostream>

MatrixView::MatrixView(const Matrix &m, const long start_row, const long start_col, const long num_rows, const long num_cols)
    : Matrix(num_rows, start_row, m.cols(), start_col, num_cols, m.get_data())
{
    if (start_row + num_rows > m.rows() || start_col + num_cols > m.cols())
    {
        throw std::invalid_argument("MatrixView: invalid dimensions");
    }
}

MatrixView::MatrixView(const MatrixView &m)
    : Matrix(m._rows, m._rows_start, m.cols(), m._cols_start, m.reported_colomns, m.get_data())
{
}

MatrixView::MatrixView(MatrixView &&m)
    : Matrix(m._rows, m._rows_start, m.cols(), m._cols_start, m.reported_colomns, m.get_data())
{
    m._rows = 0;
    m._rows_start = 0;
    m._cols = 0;
    m._cols_start = 0;
    m.reported_colomns = 0;
    m.data = nullptr;
}

MatrixView::~MatrixView()
{
}

MatrixView &MatrixView::operator=(const MatrixView &m)
{
    if (this != &m)
    {
        data = nullptr;
        _rows = m._rows;
        _rows_start = m._rows_start;
        _cols = m._cols;
        _cols_start = m._cols_start;
        reported_colomns = m.reported_colomns;
        data = m.data;
    }
    return *this;
}

MatrixView &MatrixView::operator=(MatrixView &&m)
{
    if (this != &m)
    {
        data = nullptr;
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

double &MatrixView::operator()(const long i, const long j)
{
    if (i < 0 || i >= _rows || j < 0 || j >= reported_colomns)
    {
        throw std::out_of_range("MatrixView: index out of range");
    }
    // std::cout << (_rows_start + i) * _cols + (_cols_start + j) << std::endl; 
    return data[(_rows_start + i) * _cols + (_cols_start + j)];
}

double MatrixView::operator()(const long i, const long j) const
{
    if (i < 0 || i >= _rows || j < 0 || j >= reported_colomns)
    {
        throw std::out_of_range("MatrixView: index out of range");
    }
    // std::cout << (_rows_start + i) * _cols + (_cols_start + j) << std::endl; 
    return data[(_rows_start + i) * _cols + (_cols_start + j)];
}

long MatrixView::rows() const
{
    return _rows;
}

long MatrixView::cols() const
{
    return reported_colomns;
}

void MatrixView::print() const
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

