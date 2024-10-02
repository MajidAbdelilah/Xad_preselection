#include "MatrixView.hpp"
#include <iostream>
#include <thread>

MatrixView::MatrixView(Matrix &m, const long start_row, const long start_col, const long num_rows, const long num_cols)
    : _rows(num_rows), _rows_start(start_row), _cols(m.cols()), _cols_start(start_col), _reported_cols(num_cols), data(m.get_data()), data_ref_count(m.get_data_ref_count())
{
    (*data_ref_count)++;
}

MatrixView::MatrixView(const MatrixView &m)
    : _rows(m._rows), _rows_start(m._rows_start), _cols(m._cols), _cols_start(m._cols_start), _reported_cols(m._reported_cols), data(m.data), data_ref_count(m.data_ref_count)
{
    (*data_ref_count)++;
}

MatrixView::MatrixView(MatrixView &&m)
    : _rows(m._rows), _rows_start(m._rows_start), _cols(m._cols), _cols_start(m._cols_start), _reported_cols(m._reported_cols), data(m.data), data_ref_count(m.data_ref_count)
{
    m.data = nullptr;
    m.data_ref_count = nullptr;
}

MatrixView::~MatrixView()
{
    if (--(*data_ref_count) == 0)
    {
        delete[] data;
        delete data_ref_count;
    }
}

MatrixView &MatrixView::operator=(const MatrixView &m)
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
    _rows_start = m._rows_start;
    _cols = m._cols;
    _cols_start = m._cols_start;
    _reported_cols = m._reported_cols;
    data = m.data;
    data_ref_count = m.data_ref_count;
    (*data_ref_count)++;
    return *this;
}

MatrixView &MatrixView::operator=(MatrixView &&m)
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
    _rows_start = m._rows_start;
    _cols = m._cols;
    _cols_start = m._cols_start;
    _reported_cols = m._reported_cols;
    data = m.data;
    data_ref_count = m.data_ref_count;
    return *this;
}

double &MatrixView::operator()(const long i, const long j)
{
    if(i < 0 || i >= _rows || j < 0 || j >= _reported_cols)
    {
        throw std::out_of_range("Index out of range");
    }
    return data[(i + _rows_start) * _cols + j + _cols_start];
}

double MatrixView::operator()(const long i, const long j) const
{
    if(i < 0 || i >= _rows || j < 0 || j >= _reported_cols)
    {
        throw std::out_of_range("Index out of range");
    }
    return data[(i + _rows_start) * _cols + j + _cols_start];
}

long MatrixView::rows() const
{
    return _rows;
}

long MatrixView::cols() const
{
    return _reported_cols;
}

void MatrixView::print() const
{
    for (long i = 0; i < _rows; i++)
    {
        for (long j = 0; j < _reported_cols; j++)
        {
            std::cout << data[(i + _rows_start) * _cols + j + _cols_start] << " ";
        }
        std::cout << std::endl;
    }
}

double *MatrixView::get_data() const
{
    return data;
}

long *MatrixView::get_data_ref_count() const
{
    return data_ref_count;
}

void copy_cols_thread(double *m_data, unsigned long size, long _reported_cols, const double *src, long _rows_start, long _cols_start, long _cols)
{
    for (long i = 0; i < size; i++)
    {
        // memcpy(&m_data[i * m_cols], &src[(i + _rows_start) * _cols + _cols_start], _reported_cols * sizeof(double));
        
        for (long j = 0; j < _reported_cols; j++)
        {
            m_data[i * _reported_cols + j] = src[(i + _rows_start) * _cols + j + _cols_start];
            // m(i, j) = data[(i + _rows_start) * _cols + j + _cols_start];
        }
        
        // for (long j = 0; j < _reported_cols; j++)
        // {
        //     m.get_data()[i * m.cols() + j] = data[(i + _rows_start) * _cols + j + _cols_start];
        //     m(i, j) = data[(i + _rows_start) * _cols + j + _cols_start];
        // }
    }
}

MatrixView::operator Matrix() const
{
    Matrix m(_rows, _reported_cols);

    // auto processor_count = std::thread::hardware_concurrency();
    // std::thread threads[processor_count];
    // unsigned long size = _rows;
    // unsigned long sizePerThread = size / processor_count;

    // for (unsigned long i = 0; i < processor_count; ++i)
    // {
    //     const double *src = data;
    //     double *m_data = m.get_data() + i * sizePerThread * _reported_cols;
    //     if (i == processor_count - 1)
    //     {
    //         unsigned long rest = size - i * sizePerThread;
    //         threads[i] = std::thread(copy_cols_thread, m_data, rest, _reported_cols, src, _rows_start + i * sizePerThread, _cols_start, _cols);
    //         break;
    //     }
    //     threads[i] = std::thread(copy_cols_thread, m_data, sizePerThread, _reported_cols, src, _rows_start + i * sizePerThread, _cols_start, _cols);
    // }
    
    for (long i = 0; i < _rows; i++)
    {
        memcpy(&m.get_data()[i * m.cols()], &data[(i + _rows_start) * _cols + _cols_start], _reported_cols * sizeof(double));
        // for (long j = 0; j < _reported_cols; j++)
        // {
        //     m.get_data()[i * m.cols() + j] = data[(i + _rows_start) * _cols + j + _cols_start];
        //     m(i, j) = data[(i + _rows_start) * _cols + j + _cols_start];
        // }
    }

    return m;
}

