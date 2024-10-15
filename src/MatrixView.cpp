#include "MatrixView.hpp"
#include <iostream>
#include <thread>
#include <string.h>

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

void copy_MatrixView_data_to_Matrix_threaded(const MatrixView &mv, Matrix &m, const long start, const long end)
{
	for (long i = start; i < end; i++)
	{
        memcpy(&m.get_data()[i * m.cols()], &mv.get_data()[(i + mv.get_rows_start()) * mv.get_true_cols() + mv.get_cols_start()], mv.get_reported_cols() * sizeof(double));
	}
}

long MatrixView::get_cols_start() const
{
	return _cols_start;
}

long MatrixView::get_rows_start() const
{
	return _rows_start;
}

long MatrixView::get_reported_cols() const
{
	return _reported_cols;
}

long MatrixView::get_true_cols() const
{
	return _cols;
}

#ifdef un_optimized
MatrixView::operator Matrix() const
{
    Matrix m(_rows, _reported_cols);
	for (long i = 0; i < _rows; i++)
	{
		for(long j = 0; j < _reported_cols; j++)
	    	m.get_data()[i * m.cols() + j] = data[(i + _rows_start) * _cols + j + _cols_start];
	}
	return m;
}
#else
MatrixView::operator Matrix() const
{
    Matrix m(_rows, _reported_cols);
	auto thread_count = std::thread::hardware_concurrency();
	if(thread_count == 0)
		thread_count = 1;

	if((thread_count) > _rows)
		thread_count = _rows;
	auto thread_size = _rows / thread_count;
	std::thread *threads = new std::thread[thread_count];
	for (long i = 0; i < thread_count; i++)
	{
		long start = i * thread_size;
		long end = (i == thread_count - 1) ? _rows : (i + 1) * thread_size;
		threads[i] = std::thread(copy_MatrixView_data_to_Matrix_threaded, std::ref(*this), std::ref(m), start, end);
	}
	for (long i = 0; i < thread_count; i++)
	{
		threads[i].join();
	}
    delete [] threads;
	// for (long i = 0; i < _rows; i++)
	// {
	//     memcpy(&m.get_data()[i * m.cols()], &data[(i + _rows_start) * _cols + _cols_start], _reported_cols * sizeof(double));
	// }
	return m;
}
#endif