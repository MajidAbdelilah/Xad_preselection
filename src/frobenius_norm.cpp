 #include "frobenius_norm.hpp"
#include <math.h>

double frobenius_norm_baseline_unoptimized(const Matrix& m)
{
    double sum = 0;
    for (long i = 0; i < m.rows(); i++)
    {
        for (long j = 0; j < m.cols(); j++)
        {
            sum += m(i, j) * m(i, j);
        }
    }
    return sqrt(sum);
}