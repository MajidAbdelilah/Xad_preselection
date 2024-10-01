#include "Matrix.hpp"
#include <iostream>
#include "MatrixView.hpp" 

Matrix::Matrix(const long _rows, const long _cols)
    : _rows(_rows), _rows_start(0), _cols(_cols), _cols_start(0), reported_colomns(_cols), data(new double[_rows * _cols]), delete_views(true)
{
}

Matrix::Matrix(const long _rows, const long _cols, const double val)
    : _rows(_rows), _rows_start(0), _cols(_cols), _cols_start(0), reported_colomns(_cols), data(new double[_rows * _cols]), delete_views(true)
{
    for (long i = 0; i < _rows * _cols; i++)
    {
        data[i] = val;
    }
}

Matrix::Matrix(const Matrix &m)
    : _rows(m._rows), _cols(m.reported_colomns), _rows_start(0), _cols_start(0), reported_colomns(m.reported_colomns), data(new double[m._rows * m.reported_colomns]), delete_views(m.delete_views)
{
    for(int i = 0; i < m._rows; i++)
    {
        for(int j = 0; j < m.reported_colomns; j++)
        {
            data[i * m.reported_colomns + j] = m(i, j);
        }
    }
}

Matrix::Matrix(Matrix &&m)
    : _rows(m._rows), _cols(m._cols), _rows_start(m._rows_start), _cols_start(m._cols_start), reported_colomns(m.reported_colomns), data(m.data), views(m.views), delete_views(m.delete_views)
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
    std::cout << "Matrix destructor" << std::endl;

    if(delete_views)
    {
        if(!views.size())
        {
            delete [] data;
        }
    }
   
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
        views = m.views;
        for (long i = 0; i < m._rows * m._cols; i++)
        {
            data[i] = m.data[i];
            views[i] = m.views[i];
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
    if (i < 0 || i >= _rows || j < 0 || j >= reported_colomns)
    {
        throw std::out_of_range("MatrixView: index out of range");
    }
    // std::cout << (_rows_start + i) * _cols + (_cols_start + j) << std::endl; 
    return data[(_rows_start + i) * _cols + (_cols_start + j)];
}

double Matrix::operator()(const long i, const long j) const
{
    if (i < 0 || i >= _rows || j < 0 || j >= reported_colomns)
    {
        throw std::out_of_range("MatrixView: index out of range");
    }
    // std::cout << (_rows_start + i) * _cols + (_cols_start + j) << std::endl; 
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


Matrix::Matrix(const long _rows, const long _rows_start, const long _cols, const long _cols_start, const long reported_colomns, double *data)
    : _rows(_rows), _rows_start(_rows_start), _cols(_cols), _cols_start(_cols_start), reported_colomns(reported_colomns), data(data)
{

}

double *Matrix::get_data() const
{
    return data;
}

std::vector<MatrixView*> &Matrix::get_views()
{
    return views;
}

