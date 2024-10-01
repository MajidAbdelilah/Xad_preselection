#include "Matrix.hpp"

class MatrixView : public Matrix
{
public:
    MatrixView(const Matrix &m, const long start_row, const long start_col, const long num_rows, const long num_cols);
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
};