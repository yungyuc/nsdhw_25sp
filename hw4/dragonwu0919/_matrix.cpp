// this file is a copy from exmaple code in class note
// https://yyc.solvcon.net/en/latest/nsd/05cache/example.html#nsd-cache-example-03-matrix-matri#include "stopWatch.hpp"

#include "_matrix.hpp"


template struct MyAllocator<double>;

Matrix Matrix::transpose() const
{
    Matrix ret(nrow(), ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t j=0; j<ret.ncol(); ++j)
        {
            ret(j, i) = (*this)(i, j);
        }
    }

    return ret;
}

bool operator== (Matrix const & mat1, Matrix const & mat2)
{
    if ((mat1.ncol() != mat2.ncol()) && (mat1.nrow() != mat2.ncol()))
    {
        return false;
    }

    for (size_t i=0; i<mat1.nrow(); ++i)
    {
        for (size_t j=0; j<mat1.ncol(); ++j)
        {
            if (mat1(i, j) != mat2(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

bool operator!= (Matrix const & mat1, Matrix const & mat2)
{
    return !(mat1 == mat2);
}

/*
 * Throw an exception if the shapes of the two matrices don't support
 * multiplication.
 */
void validate_multiplication(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

/*
 * Get the number of floating-point operations.
 */
size_t calc_nflo(Matrix const & mat1, Matrix const & mat2)
{
    return mat1.nrow() * mat1.ncol() * mat2.ncol();
}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
#if !defined(HASMKL) || defined(NOMKL)
    // run with VECLIB_MAXIMUM_THREADS=1
#else // HASMKL NOMKL
    mkl_set_num_threads(1);
#endif // HASMKL NOMKL

    Matrix ret(mat1.nrow(), mat2.ncol());

    StopWatch sw;

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer.data() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer.data() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer.data() /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
}

/*
 * Indirect naive matrix matrix multiplication.
 */
Matrix multiply_indirect(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    StopWatch sw;

    for (size_t i=0; i<mat1.nrow(); ++i)
    {
        for (size_t k=0; k<mat2.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
}

/*
 * Indirect naive matrix matrix multiplication but loop in a different order
 * (i,j,k).  It is much faster than the original order (i,k,j), because mat2
 * now has a much better locality.
 */
Matrix multiply_indirect_order1(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    StopWatch sw;

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t j=0; j<ret.ncol(); ++j)
        {
            ret(i,j) = 0;
        }
    }

    for (size_t i=0; i<mat1.nrow(); ++i)
    {
        for (size_t j=0; j<mat1.ncol(); ++j)
        {
            for (size_t k=0; k<mat2.ncol(); ++k)
            {
                ret(i,k) += mat1(i,j) * mat2(j,k);
            }
        }
    }

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
}

/*
 * Direct naive matrix matrix multiplication.
 */
Matrix multiply_direct(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    StopWatch sw;

    for (size_t i=0; i<nrow1; ++i)
    {
        const size_t base1 = i * ncol1;
        for (size_t k=0; k<ncol2; ++k)
        {
            double v = 0;
            for (size_t j=0; j<ncol1; ++j)
            {
                v += mat1.m_buffer[base1 + j] * mat2.m_buffer[j*ncol2 + k];
            }
            ret.m_buffer[base1 + k] = v;
        }
    }

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
}

double waste_time(){
    double d = 0;
    for(int i=0; i<2; i++){
        d = d-1;
        d = i*d;
        d = i^2;
    }
    return d;
}


Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    StopWatch sw;

    for (size_t i=0; i<nrow1; ++i)
    {
        const size_t base1 = i * ncol1;
        for (size_t k=0; k<ncol2; ++k)
        {
            double v = 0;
            double damp = waste_time();
            for (size_t j=0; j<ncol1; ++j)
            {
                v += mat1.m_buffer[base1 + j] * mat2.m_buffer[j*ncol2 + k];
                v *= damp;
                v /= damp;

            }
            ret.m_buffer[base1 + k] = v;
        }
    }

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
}


template<size_t N> void Block<N>::save(
    Matrix & mat, size_t it, size_t jt
)
{
    const size_t ncol = mat.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (it*NDIM + i) * ncol + jt*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            mat.m_buffer[base_t + j] = m_buffer[base_s + j];
        }
    }
}

template<size_t N> void Tiler<N>::load(
    Matrix const & mat1, size_t it1, size_t jt1
  , Matrix const & mat2, size_t it2, size_t jt2
)
{
    const size_t ncol1 = mat1.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it1*NDIM + i) * ncol1 + jt1*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat1[base_t + j] = mat1.m_buffer[base_s + j];
        }
    }

    const size_t ncol2 = mat2.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it2*NDIM + i) * ncol2 + jt2*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_ret[base_t + j] = mat2.m_buffer[base_s + j];
        }
    }

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base = i*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat2[j*NDIM + i] = m_ret[base + j];
        }
    }
}

template<size_t N> void Tiler<N>::multiply()
{
    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base1 = i*NDIM;

        for (size_t k=0; k<NDIM; ++k)
        {
            const size_t base2 = k*NDIM;

            double v = 0;
            for (size_t j=0; j<NDIM; ++j)
            {
                v += m_mat1[base1 + j] * m_mat2[base2 + j];
            }
            m_ret[base1 + k] = v;
        }
    }
}

template<size_t tsize>
void show_block(Block<tsize> & block){
    std::cout << std::string("=",tsize*5) << std::endl;
    for(size_t i=0; i < tsize; i++){
        for(size_t j=0; j<tsize; j++){
            std::cout << block[i*tsize +j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::string("=",tsize*5) << std::endl;
}


/*
 * Tiled matrix matrix multiplication.
 */
template<size_t LSIZE>
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    constexpr const size_t tsize = LSIZE / sizeof(double);

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    const size_t ntrow1 = nrow1 / tsize;
    const size_t ntcol1 = ncol1 / tsize;
    const size_t ntrow2 = nrow2 / tsize;
    const size_t ntcol2 = ncol2 / tsize;

    Block<tsize> value;
    Tiler<tsize> tiler;

    StopWatch sw;

    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t kt=0; kt<ntcol2; ++kt)
        {   
            value = 0;
            for (size_t jt=0; jt<ntcol1; ++jt)
            {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply();
                value += tiler.m_ret;
            }
            show_block<tsize>(value);
            value.save(ret, it, kt);
        }
    }

    ret.elapsed() = sw.lap();
    ret.nflo() = calc_nflo(mat1, mat2);

    return ret;
}

template Matrix multiply_tile<128>(Matrix const & mat1, Matrix const & mat2);

void initialize(Matrix & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            if (0 == i%2)
            {
                mat(i, j) = j;
            }
            else
            {
                mat(i, j) = mat.ncol() - 1 - j;
            }
        }
    }
}

template<size_t TSIZE>
Matrix time_tile(
    std::string tag, Matrix const * gold
  , Matrix const & mat1, Matrix const & mat2
)
{
    std::function<Matrix (Matrix const &, Matrix const &)> runner;

    if (sizeof(double) == TSIZE)
    {
        if ("mkl" == tag)
        {
            runner = multiply_mkl;
        }
        else if ("indirect" == tag)
        {
            runner = multiply_indirect;
        }
        else if ("indirect_order1" == tag)
        {
            runner = multiply_indirect_order1;
        }
        else if ("direct" == tag)
        {
            runner = multiply_direct;
        }
        else
        {
            throw std::runtime_error("invalid tag");
        }
    }
    else
    {
        std::ostringstream t;
        t << "tiled " << TSIZE;
        tag = t.str();
        runner = multiply_tile<TSIZE>;
    }

    std::cout << "Timing " << tag << ": ";

    Matrix res = runner(mat1, mat2);

    if (gold && (res != *gold))
    {
        throw std::runtime_error("answer mismatch");
    }

    std::cout << res.elapsed() << " second, "
              << res.nflo()/1.e9 << " Gflo, "
              << res.gflops() << " Gflops"
              << std::endl;

    return res;
}

template Matrix time_tile<2>(std::string, Matrix const*, Matrix const&, Matrix const&);
template Matrix time_tile<8>(std::string, Matrix const*, Matrix const&, Matrix const&);
template Matrix time_tile<32>(std::string, Matrix const*, Matrix const&, Matrix const&);
template Matrix time_tile<64>(std::string, Matrix const*, Matrix const&, Matrix const&);
template Matrix time_tile<128>(std::string, Matrix const*, Matrix const&, Matrix const&);
template Matrix time_tile<256>(std::string, Matrix const*, Matrix const&, Matrix const&);
template Matrix time_tile<512>(std::string, Matrix const*, Matrix const&, Matrix const&);
template Matrix time_tile<1024>(std::string, Matrix const*, Matrix const&, Matrix const&);


Matrix simple_tile(Matrix const & mat1, Matrix const & mat2, size_t lsize){

    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    size_t tsize = lsize;  
    

    size_t nrow1 = mat1.nrow();
    size_t ncol1 = mat1.ncol();
    size_t nrow2 = mat2.nrow();
    size_t ncol2 = mat2.ncol();

    size_t ntrow1 = nrow1 / tsize + 1;
    size_t ntcol1 = ncol1 / tsize + 1;
    size_t ntcol2 = ncol2 / tsize + 1;

    for (size_t it = 0; it < ntrow1; ++it)
    {
        for (size_t kt = 0; kt < ntcol2; ++kt)
        {
            // 初始化 block (tsize x tsize) 為 0
            std::vector<double> block(tsize * tsize, 0.0);

            for (size_t jt = 0; jt < ntcol1; ++jt)
            {
                // 進行一個 block 的乘法：block += A_tile * B_tile
                for (size_t i = 0; i < tsize; ++i)
                {
                    for (size_t j = 0; j < tsize; ++j)
                    {
                        for (size_t k = 0; k < tsize; ++k)
                        {
                            double a = mat1(it * tsize + i, jt * tsize + k);
                            double b = mat2(jt * tsize + k, kt * tsize + j);
                            block[i*tsize + j] += a * b;
                        }
                    }
                }
            }

            // 存回結果矩陣
            for (size_t i = 0; i < tsize; ++i)
            {
                for (size_t j = 0; j < tsize; ++j)
                {
                    ret(it * tsize + i, kt * tsize + j) = block[i*tsize + j];
                }
            }
        }
    }


    return ret;
}