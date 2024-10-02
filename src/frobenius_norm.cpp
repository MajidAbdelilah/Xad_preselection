 #include "frobenius_norm.hpp"
#include <cmath>
#include <math.h>
#include <thread>

#include <immintrin.h>
#include <iostream>
#include <assert.h>

// CPUs support RAM access like this: "ymmword ptr [rax+64]"
// Using templates with offset int argument to make easier for compiler to emit good code.

// Multiply 4 doubles by another 4 doubles.
template<int offsetRegs>
inline __m256d mul4( const double* p1, const double* p2 )
{
    constexpr int lanes = offsetRegs * 4;
    const __m256d a = _mm256_loadu_pd( p1 + lanes );
    const __m256d b = _mm256_loadu_pd( p2 + lanes );
    return _mm256_mul_pd( a, b );
}

// Returns acc + ( p1 * p2 ), for 4-wide double lanes.
template<int offsetRegs>
inline __m256d fma4( __m256d acc, const double* p1, const double* p2 )
{
    constexpr int lanes = offsetRegs * 4;
    const __m256d a = _mm256_loadu_pd( p1 + lanes );
    const __m256d b = _mm256_loadu_pd( p2 + lanes );
    return _mm256_fmadd_pd( a, b, acc );
}

// Compute dot product of double vectors, using 4-wide FMA instructions.
double dotProductFma( const double *a, const unsigned long a_size, const double *b, const unsigned long b_size )
{
    assert( a_size == b_size );
    assert( 0 == ( a_size % 16 ) );
    if( a_size == 0 )
        return 0.0f;

    const double* p1 = a;
    const double* const p1End = p1 + a_size;
    const double* p2 = b;

    // Process initial 16 values. Nothing to add yet, just multiplying.
    __m256d dot0 = mul4<0>( p1, p2 );
    __m256d dot1 = mul4<1>( p1, p2 );
    __m256d dot2 = mul4<2>( p1, p2 );
    __m256d dot3 = mul4<3>( p1, p2 );
    p1 += 4 * 4;
    p2 += 4 * 4;

    // Process the rest of the data.
    // The code uses FMA instructions to multiply + accumulate, consuming 16 values per loop iteration.
    // Unrolling manually for 2 reasons:
    // 1. To reduce data dependencies. With a single register, every loop iteration would depend on the previous result.
    // 2. Unrolled code checks for exit condition 4x less often, therefore more CPU cycles spent computing useful stuff.
    while( p1 < p1End )
    {
        dot0 = fma4<0>( dot0, p1, p2 );
        dot1 = fma4<1>( dot1, p1, p2 );
        dot2 = fma4<2>( dot2, p1, p2 );
        dot3 = fma4<3>( dot3, p1, p2 );
        p1 += 4 * 4;
        p2 += 4 * 4;
    }

    // Add 16 values into 4
    const __m256d dot01 = _mm256_add_pd( dot0, dot1 );
    const __m256d dot23 = _mm256_add_pd( dot2, dot3 );
    const __m256d dot0123 = _mm256_add_pd( dot01, dot23 );
    // Add 4 values into 2
    const __m128d r4 = _mm_add_pd( _mm256_castpd256_pd128( dot0123 ), _mm256_extractf128_pd( dot0123, 1 ) );
    
    double tmp[2];
    _mm_storeu_pd( tmp, r4 );
    return tmp[0] + tmp[1];
}



double frobenius_norm_thread(const double *m, const unsigned long size)
{
    // const unsigned long size = m.rows() * m.cols();
    const unsigned long sizeAligned = size / 16 * 16;
    const double* data = m;

    const double sum = dotProductFma( data, sizeAligned, data, sizeAligned );

    // Process the rest of the data.
    double rest = 0.0;
    for( unsigned long i = sizeAligned; i < size; ++i )
    {
        rest += data[i] * data[i];
    }

    return sum + rest;
}

double frobenius_norm(const Matrix& m)
{
    auto processor_count = std::thread::hardware_concurrency();
	if(processor_count == 0)
		processor_count = 1;
    // std::cout << "Processor count: " << processor_count << std::endl;
    const unsigned long size = m.rows() * m.cols();
	if((processor_count * 16) > size)
	{
		processor_count = size / 16;
		if(processor_count == 0)
			processor_count = 1;
	}
    const unsigned long sizePerThread = size / processor_count;
    std::thread threads[processor_count];
    double results[processor_count];


    for( unsigned long i = 0; i < processor_count; ++i )
    {
        const double* data = m.get_data() + i * sizePerThread;
        if( i == processor_count - 1 )
        {
            unsigned long rest = size - i * sizePerThread;
            // std::cout << "Rest: " << rest << std::endl;
            threads[i] = std::thread( [&results, i, data, rest] {
                results[i] = frobenius_norm_thread( data, rest );
            } );
            break;
        }
        threads[i] = std::thread( [&results, i, data, sizePerThread] {
            results[i] = frobenius_norm_thread( data, sizePerThread );
        } );
    }

    for( unsigned long i = 0; i < processor_count; ++i )
    {
        threads[i].join();
    }

    double sum = 0.0;

    for( unsigned long i = 0; i < processor_count; ++i )
    {
        sum += results[i];
    }

    return sqrt( sum );
}




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