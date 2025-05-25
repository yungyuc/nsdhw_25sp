#include "stopWatch.hpp"

#ifdef HASMKL
#include <mkl.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>
#else // HASMKL
#ifdef __MACH__
#include <clapack.h>
#include <Accelerate/Accelerate.h>
#endif // __MACH__
#endif // HASMKL

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>
#include <cblas.h>



struct Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_elapsed(other.m_elapsed), m_nflo(other.m_nflo)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        m_elapsed = other.m_elapsed;
        m_nflo = other.m_nflo;
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_elapsed(other.m_elapsed), m_nflo(other.m_nflo)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        std::swap(m_elapsed, other.m_elapsed);
        std::swap(m_nflo, other.m_nflo);
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { 
        static double trash_can = 0.;
        if (row >= m_nrow || col >= m_ncol) {
            return trash_can;
        }
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)       { 
        static double trash_can = 0.;

        if(trash_can != 0) trash_can = 0; 

        if (row >= m_nrow || col >= m_ncol) {
            return trash_can;
            // std::ostringstream oss;
            // oss << "Matrix access out of bounds: row_index=" << row << ", col_index=" << col
            //     << " (max_row=" << m_nrow -1 << ", max_col=" << m_ncol -1 << ")";
            // throw std::out_of_range(oss.str());
        }
        return m_buffer[index(row, col)]; 
    }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

    double   elapsed() const { return m_elapsed; }
    double & elapsed()       { return m_elapsed; }

    size_t   nflo() const { return m_nflo; }
    size_t & nflo()       { return m_nflo; }

    double gflops() const { return m_nflo / m_elapsed / 1.e9; }

    Matrix transpose() const;

public:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { 
            m_buffer = new double[nelement];
            std::fill(m_buffer, m_buffer+nelement, 0); 
        }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;
    double m_elapsed = 0;
    size_t m_nflo = 0; // number of floating-point operations.

};

bool operator== (Matrix const & mat1, Matrix const & mat2);

bool operator!= (Matrix const & mat1, Matrix const & mat2);
/*
 * Throw an exception if the shapes of the two matrices don't support
 * multiplication.
 */
void validate_multiplication(Matrix const & mat1, Matrix const & mat2);

/*
 * Get the number of floating-point operations.
 */
size_t calc_nflo(Matrix const & mat1, Matrix const & mat2);

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

/*
 * Indirect naive matrix matrix multiplication.
 */
Matrix multiply_indirect(Matrix const & mat1, Matrix const & mat2);

/*
 * Indirect naive matrix matrix multiplication but loop in a different order
 * (i,j,k).  It is much faster than the original order (i,k,j), because mat2
 * now has a much better locality.
 */
Matrix multiply_indirect_order1(Matrix const & mat1, Matrix const & mat2);

/*
 * Direct naive matrix matrix multiplication.
 */
Matrix multiply_direct(Matrix const & mat1, Matrix const & mat2);

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);

template<size_t N>
struct Block
{
    static constexpr const size_t NDIM = N;

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    Block<N> & operator= (double v)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] = v; }
        return *this;
    }

    Block<N> & operator+= (Block<N> const & other)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] += other.m_buffer[i]; }
        return *this;
    }

    void save(Matrix & mat, size_t it, size_t jt);

    double m_buffer[N * N];
};

template<size_t N>
struct Tiler
{
    static constexpr const size_t NDIM = N;

    void load(
        Matrix const & mat1, size_t it1, size_t jt1
      , Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

    Block<N> m_mat1; // row-major
    Block<N> m_mat2; // column-major
    Block<N> m_ret; // row-major
};

/*
 * Tiled matrix matrix multiplication.
 */
template<size_t LSIZE>
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2);

void initialize(Matrix & mat);

template<size_t TSIZE=sizeof(double)>
Matrix time_tile(
    std::string tag, Matrix const * gold
  , Matrix const & mat1, Matrix const & mat2
);

extern template Matrix multiply_tile<128>(Matrix const & mat1, Matrix const & mat2);


Matrix simple_tile(Matrix const & mat1, Matrix const & mat2, size_t lsize);