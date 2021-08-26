/* -------------------------------------------------------------------------- *
 *                        Simbody(tm): SimTKmath                              *
 * -------------------------------------------------------------------------- *
 * This is part of the SimTK biosimulation toolkit originating from           *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org/home/simbody.  *
 *                                                                            *
 * Portions copyright (c) 2007-12 Stanford University and the Authors.        *
 * Authors: Jack Middleton                                                    *
 * Contributors:                                                              *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

/**@file
 * These is a templatized, C++ callable interface to LAPACK and BLAS.
 * Each method must be explicitly specialized for the supported precisions.
 */


#include "SimTKcommon.h"
#include "SimTKlapack.h"
#include "SimTKmath.h"
#include "LapackInterface.h"
#include "WorkSpace.h"
#include <cstring>
#include <stdexcept>

#ifdef _MSC_VER
#pragma warning(disable:4996) // don't warn about strcat, sprintf, etc.
#endif



static const double EPS = .000001;
namespace SimTK {

int LapackInterface::getLWork( float* work) { return( (int)work[0] ); }
int LapackInterface::getLWork( double* work) { return( (int)work[0] ); }
int LapackInterface::getLWork( std::complex<float>* work) { return( (int)work[0].real() ); }
int LapackInterface::getLWork( std::complex<double>* work) { return( (int)work[0].real() ); }

template <typename T> void LapackInterface::gelss( int m, int n,  int mn, int nrhs,
           T* a, int lda, T* b, int ldb,  typename CNT<T>::TReal* s,
           typename CNT<T>::TReal rcond, int& rank, int& info){ assert(false); }

template <> void LapackInterface::gelss<double>( int m, int n,  int mn, int nrhs,
           double* a, int lda, double* b,  int ldb, double* s,
           double rcond, int& rank, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gelss called"));
#else SIMBODY_WITHOUT_LAPACK
    double wsize[1];
    dgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, wsize, -1, info );

    int lwork = (int)wsize[0];
    TypedWorkSpace<double> work(lwork);

    dgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, work.data, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dgelss", info );
    }
#endif
}

template <> void LapackInterface::gelss<float>( int m, int n,  int mn, int nrhs,
           float* a, int lda, float* b, int ldb,   float* s,
           float rcond, int& rank, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gelss called"));
#else SIMBODY_WITHOUT_LAPACK
    float wsize[1];
    sgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, wsize, -1, info );

    int lwork = (int)wsize[0];
    TypedWorkSpace<float> work(lwork);

    sgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, work.data, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "sgelss", info );
    }
#endif
}

template <> void LapackInterface::gelss<std::complex<float> >( int m, int n,  int mn, int nrhs,
           std::complex<float>* a, int lda, std::complex<float>* b,  int ldb, float* s,
           float rcond, int& rank, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gelss called"));
#else SIMBODY_WITHOUT_LAPACK
    std::complex<float>  wsize[1];
    TypedWorkSpace<float> rwork(5*mn);
    cgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, wsize, -1, rwork.data, info );

    int lwork = (int)wsize[0].real();
    TypedWorkSpace<std::complex<float> > work(lwork);

    cgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, work.data, lwork, rwork.data, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cgelss", info );
    }
#endif
}

template <> void LapackInterface::gelss<std::complex<double> >( int m, int n,  int mn, int nrhs,
           std::complex<double>* a, int lda, std::complex<double>* b,  int ldb, double* s,
           double rcond, int& rank, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gelss called"));
#else SIMBODY_WITHOUT_LAPACK
    TypedWorkSpace<double> rwork(5*mn);
    std::complex<double>  wsize[1];
    zgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, wsize, -1, rwork.data, info );

    int lwork = (int)wsize[0].real();
    TypedWorkSpace<std::complex<double> > work(lwork);

    zgelss_(m, n, nrhs, a, lda, b, ldb, s, rcond, rank, work.data, lwork, rwork.data, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zgelss", info );
    }
#endif
}


template <> void LapackInterface::potrs<double>
    ( char uplo, const int ncol, const int nrhs, const double *lu,  double *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    dpotrs_(uplo, ncol, nrhs, lu, ncol, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dpotrs", info );
    }
#endif

    return;
}

template <> void LapackInterface::potrs<float>
    ( char uplo, const int ncol, const int nrhs, const float *lu,  float *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    spotrs_(uplo, ncol, nrhs, lu, ncol, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "spotrs", info );
    }
#endif

    return;
}

template <> void LapackInterface::potrs<std::complex<float> >
    ( char uplo, const int ncol, const int nrhs, const std::complex<float>* lu,  std::complex<float>* b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    cpotrs_(uplo, ncol, nrhs, lu, ncol, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cpotrs", info );
    }
#endif

    return;
}

template <> void LapackInterface::potrs<std::complex<double> >
    ( char uplo, const int ncol, const int nrhs, const std::complex<double>* lu,  std::complex<double>* b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    zpotrs_(uplo, ncol, nrhs, lu, ncol, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zpotrs", info );
    }
#endif

    return;
}
template <> void LapackInterface::sytrs<double>
// TODO fix SimTKlapack.h for const int* pivots    ( char trans,  const int ncol, const int nrhs, const double *lu, const int *pivots, double *b ) {
( char trans,  const int ncol, const int nrhs, double *lu,  int *pivots, double *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    dsytrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dsytrs", info );
    }
#endif

    return;
}

template <> void LapackInterface::sytrs<float>
// TODO    ( char trans, const int ncol, const int nrhs, const float *lu, const int *pivots, float *b ) {
    ( char trans, const int ncol, const int nrhs, float *lu, int *pivots, float *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    ssytrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "ssytrs", info );
    }
#endif

    return;
}

template <> void LapackInterface::sytrs<std::complex<float> >
// TODO    ( char trans, const int ncol, const int nrhs, const std::complex<float>* lu, const int *pivots, std::complex<float>* b ) {
    ( char trans, const int ncol, const int nrhs, std::complex<float>* lu, int *pivots, std::complex<float>* b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    chetrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "chetrs", info );
    }
#endif

    return;
}


template <> void LapackInterface::sytrs<std::complex<double> >
// TODO    ( char trans, const int ncol, const int nrhs, const std::complex<double>* lu, const int *pivots, std::complex<double>* b ) {
    ( char trans, const int ncol, const int nrhs, std::complex<double>* lu, int *pivots, std::complex<double>* b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    zhetrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zhetrs", info );
    }
#endif

    return;
}

template <> void LapackInterface::getrs<double>
    ( char trans, const int ncol, const int nrhs, const double *lu, const int *pivots, double *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    dgetrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );
  
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dgetrs", info );
    }
#endif

    return;
}


template <> void LapackInterface::getrs<float>
    ( char trans , const int ncol, const int nrhs, const float *lu, const int *pivots, float *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    sgetrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "sgetrs", info );
    }
#endif

    return;
}

template <> void LapackInterface::getrs<complex<float> >
    ( char trans, const int ncol, const int nrhs, const std::complex<float> *lu, const int *pivots, complex<float> *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    cgetrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cgetrs", info );
    }
#endif

    return;
}
template <> void LapackInterface::getrs<complex<double> >
    ( char trans, const int ncol, const int nrhs, const complex<double> *lu, const int *pivots, complex<double> *b ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrs called"));
#else SIMBODY_WITHOUT_LAPACK
    int info;

    zgetrs_(trans, ncol, nrhs, lu, ncol, pivots, b, ncol, info, 1  );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zgetrs", info );
    }
#endif

    return;
}
// selected eigenvalues for symmetric matrices
template <class T> 
void LapackInterface::syevx( char jobz, char range, char uplo, int n, T* a, int lda,
    typename CNT<T>::TReal vl, typename CNT<T>::TReal vu, int il, int iu,     
    typename CNT<T>::TReal abstol, int& nFound, typename CNT<T>::TReal* values, 
    T* vectors, int LDVectors, int* ifail, int& info ) {
    assert(false);
} 
template <> void LapackInterface::syevx<float>( char jobz, char range, 
    char uplo, int n, float* a, int lda, float vl, float vu, int il, 
    int iu,   float abstol, int& nFound, float *values, float* vectors, 
    int LDVectors, int* ifail, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syevx called"));
#else SIMBODY_WITHOUT_LAPACK
    TypedWorkSpace<int> iwork(5*n);
    float wsize[1];
    ssyevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound,
          values, vectors, LDVectors, wsize, -1, iwork.data, ifail, info, 
          1, 1, 1);

    int lwork = (int)wsize[0];
    TypedWorkSpace<float> work(lwork);
    ssyevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound,
          values, vectors, LDVectors,  work.data, lwork, iwork.data, ifail, 
          info, 1, 1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "ssyevx", info );
    }
#endif

    return;
}

template <> void LapackInterface::syevx<double>( char jobz, char range, 
    char uplo, int n, double* a, int lda, double vl, double vu, int il, 
    int iu,   double abstol, int& nFound, double *values, double* vectors, 
    int LDVectors, int* ifail, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syevx called"));
#else SIMBODY_WITHOUT_LAPACK    
    TypedWorkSpace<int> iwork(5*n);
    double wsize[1];
    dsyevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound,
          values, vectors, LDVectors, wsize, -1, iwork.data, ifail, info, 
          1, 1, 1);

    int lwork = (int)wsize[0];
    TypedWorkSpace<double> work(lwork);
    dsyevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound,
          values, vectors, LDVectors,  work.data, lwork, iwork.data, ifail, 
          info, 1, 1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dsyevx", info );
    }
#endif

    return;
}

template <> void LapackInterface::syevx<std::complex<double> >( char jobz, 
    char range, char uplo, int n, std::complex<double>* a, int lda, double vl, 
    double vu, int il, int iu,   double abstol, int& nFound, double *values, 
    std::complex<double>* vectors, int LDVectors, int* ifail, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syevx called"));
#else SIMBODY_WITHOUT_LAPACK    
    TypedWorkSpace<int> iwork(5*n);
    TypedWorkSpace<double> rwork( 7*n );
    std::complex<double>  wsize[1];
    zheevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound, 
          values, vectors, LDVectors, wsize, -1,  rwork.data, iwork.data, 
          ifail, info, 1, 1, 1);

    int lwork = (int)wsize[0].real();
    TypedWorkSpace<std::complex<double> > work(lwork);
    zheevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound,
          values, vectors, LDVectors,  work.data, lwork, rwork.data, 
          iwork.data, ifail, info, 1, 1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zheevx", info );
    }
#endif

    return;
}

template <> void LapackInterface::syevx<std::complex<float> >( char jobz, 
    char range, char uplo, int n, std::complex<float>* a, int lda, float vl, 
    float vu, int il, int iu,   float abstol, int& nFound, float *values, 
    std::complex<float>* vectors, int LDVectors, int* ifail, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syevx called"));
#else SIMBODY_WITHOUT_LAPACK      
    TypedWorkSpace<int> iwork(5*n);
    TypedWorkSpace<float> rwork( 7*n );
    std::complex<float> wsize[1];
    cheevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound,
          values, vectors, LDVectors, wsize, -1, rwork.data, iwork.data, ifail, 
          info, 1, 1, 1);

    int lwork = (int)wsize[0].real();
    TypedWorkSpace<std::complex<float> > work(lwork);
    cheevx_( jobz, range, uplo, n, a, lda, vl, vu, il, iu, abstol, nFound,
          values, vectors, LDVectors,  work.data, lwork, rwork.data, iwork.data,
          ifail, info, 1, 1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cheevx", info );
    }
#endif

    return;
}

// all eigenvalues for symmetric matrices eigen vectors returned in a
template <class T> 
void LapackInterface::syev( char jobz,  char uplo, int n, 
    T* a, int lda, typename CNT<T>::TReal * eigenValues, int& info ) {
    assert(false);
} 
template <> void LapackInterface::syev<float>( char jobz,  char uplo, int n, 
    float* a, int lda, float* eigenValues, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syev called"));
#else SIMBODY_WITHOUT_LAPACK
    float wsize[1];
    ssyev_( jobz, uplo, n, a, lda, eigenValues, wsize, -1, info,  1, 1 );
    int lwork = (int)wsize[0];
    TypedWorkSpace<float> work(lwork);
    ssyev_( jobz, uplo, n, a, lda, eigenValues, work.data, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "ssyev", info );
    }
#endif
} 

template <> void LapackInterface::syev<double>( char jobz,  char uplo, int n, 
    double* a, int lda, double* eigenValues, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syev called"));
#else SIMBODY_WITHOUT_LAPACK
    double wsize[1];
    dsyev_( jobz, uplo, n, a, lda, eigenValues, wsize, -1, info,  1, 1 );
    int lwork = (int)wsize[0];
    TypedWorkSpace<double> work(lwork);
    dsyev_( jobz, uplo, n, a, lda, eigenValues, work.data, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dsyev", info );
    }
#endif
} 

template <> void LapackInterface::syev<std::complex<float> >( char jobz,  char uplo, int n, 
    std::complex<float>* a, int lda, float* eigenValues, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syev called"));
#else SIMBODY_WITHOUT_LAPACK
    std::complex<float> wsize[1];
    int l = 3*n -2;
    if( l < 1 ) l = 1;
    
    TypedWorkSpace<float> rwork( l );

    cheev_( jobz, uplo, n, a, lda, eigenValues, wsize, -1, rwork.data, info,  1, 1 );

    int lwork = (int)wsize[0].real();
    TypedWorkSpace<std::complex<float> > work(lwork);
    cheev_( jobz, uplo, n, a, lda, eigenValues, work.data, lwork, rwork.data, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cheev", info );
    }
#endif
} 

template <> void LapackInterface::syev<std::complex<double> >( char jobz,  char uplo, int n, 
    std::complex<double>* a, int lda, double* eigenValues, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::syev called"));
#else SIMBODY_WITHOUT_LAPACK
    std::complex<double> wsize[1];
    int l = 3*n -2;
    if( l < 1 ) l = 1;
    
    TypedWorkSpace<double> rwork( l );

    zheev_( jobz, uplo, n, a, lda, eigenValues, wsize, -1, rwork.data, info,  1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zheev", info );
    }

    int lwork = (int)wsize[0].real();
    TypedWorkSpace<std::complex<double> > work(lwork);
    zheev_( jobz, uplo, n, a, lda, eigenValues, work.data, lwork, rwork.data, info, 1, 1 );

#endif
} 
template <class T> 
void LapackInterface::gesdd( char jobz, int m, int n, T* a, int lda,
           typename CNT<T>::TReal* s, T* u, int ldu,  T* vt,
           int ldvt,  int& info) {
    assert(false);
}
template <>
void LapackInterface::gesdd<float>( char jobz, int m, int n, float* a, int lda,
           float* s, float* u, int ldu,  float* vt,
           int ldvt, int& info ){
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gesdd called"));
#else SIMBODY_WITHOUT_LAPACK
    int lwork;
    int mn = (m < n ) ? m : n;  // min(m,n)
    TypedWorkSpace<float> work(1);
    TypedWorkSpace<int> iwork(8*mn);
    
    sgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, -1, iwork.data, info, 1);
    lwork = (int)work.data[0];
    work.resize(lwork); 
    sgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, lwork, iwork.data, info, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "sgesdd", info );
    }
#endif
}
template <>
void LapackInterface::gesdd<double>( char jobz, int m, int n, double* a, int lda,
           double* s, double* u, int ldu,  double* vt,
           int ldvt, int& info ){
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gesdd called"));
#else SIMBODY_WITHOUT_LAPACK
    int lwork;
    int mn = (m < n ) ? m : n;  // min(m,n)
    TypedWorkSpace<double> work(1);
    TypedWorkSpace<int> iwork(8*mn);

    dgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, -1, iwork.data, info, 1);
    lwork = (int)work.data[0];
    work.resize(lwork); 
    dgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, lwork, iwork.data, info, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dgesdd", info );
    }
#endif
}
#ifndef SIMBODY_WITHOUT_LAPACK
// TODO REMOVE when added to SimTKlapack.
extern "C" {
    extern void cgesdd_(const char& jobz, const int& m, const int& n,  std::complex<float> *a, const int& lda, float *s,  std::complex<float> *u, const int& ldu,  std::complex<float> *vt, const int& ldvt,  std::complex<float> *work, const int& lwork, float *rwork, int *iwork, int& info, int jobz_len=1 );
    extern void zgesdd_(const char& jobz, const int& m, const int& n,  std::complex<double> *a, const int& lda, double *s,  std::complex<double> *u, const int& ldu,  std::complex<double> *vt, const int& ldvt,  std::complex<double> *work, const int& lwork, double *rwork, int *iwork, int& info, int jobz_len=1 );
}
#endif
template <>
void LapackInterface::gesdd<std::complex<float> >( char jobz, int m, int n, 
      std::complex<float>* a, int lda, float* s, std::complex<float>* u, 
      int ldu,  std::complex<float>* vt, int ldvt, int& info ){
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gesdd called"));
#else SIMBODY_WITHOUT_LAPACK
    int mn = (m < n ) ? m : n;  // min(m,n)
    TypedWorkSpace<float> rwork;
    if( jobz == 'N' ) {
        rwork.resize(5*mn);
    } else {
        rwork.resize(5*mn*mn + 7*mn);
    }
    TypedWorkSpace<std::complex<float> > work(1);
    TypedWorkSpace<int> iwork(8*mn);

    cgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, -1, rwork.data, iwork.data, info, 1);
    int lwork = (int)work.data[0].real();
    work.resize(lwork); 
    cgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, lwork, rwork.data, iwork.data, info, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cgesdd", info );
    }
#endif

    return;
}

template <>
void LapackInterface::gesdd<std::complex<double> >( char jobz, int m, int n, 
      std::complex<double>* a, int lda, double* s, std::complex<double>* u, 
      int ldu,  std::complex<double>* vt, int ldvt, int& info ){
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::gesdd called"));
#else SIMBODY_WITHOUT_LAPACK
    int mn = (m < n ) ? m : n;  // min(m,n)
    TypedWorkSpace<double> rwork;
    if( jobz == 'N' ) {
        rwork.resize(5*mn);
    } else {
        rwork.resize(5*mn*mn + 7*mn);
    }
    TypedWorkSpace<std::complex<double> > work(1);
    TypedWorkSpace<int> iwork(8*mn);

    zgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, -1, rwork.data, iwork.data, info, 1);
    int lwork = (int)work.data[0].real();
    work.resize(lwork); 
    zgesdd_( jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work.data, lwork, rwork.data, iwork.data, info, 1);


    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zgesdd", info );
    }
#endif

    return;
}
// eigenvlaues for nonsymmetric matrices
template <class T> 
void LapackInterface::geev (char jobvl, char jobvr,
    int n, T* a, int lda, std::complex<typename CNT<T>::TReal>* values, 
    T* vl, int ldvl, std::complex<typename CNT<T>::TReal>* vr, 
    int ldvr, T* work, int lwork, int& info ) {
    assert(false);
}

template <> void LapackInterface::geev<double>
   (char jobvl, char jobvr,
    int n, double* a, int lda, std::complex<double>* values, 
    double* vl, int ldvl, std::complex<double>* rightVectors, int ldvr, double* work,
    int lwork, int& info )
{
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geev called"));
#else SIMBODY_WITHOUT_LAPACK
    TypedWorkSpace<double> wr(n);
    TypedWorkSpace<double> wi(n);
    TypedWorkSpace<double> vr(n*n);

    // avoid valgrind uninitialized warnings
    for(int i=0;i<n;i++) wi.data[i] = 0;  
    dgeev_( jobvl, jobvr, 
//             n, a, lda, wr.data, wi.data, vl, ldvl, vr.data, ldvr, 
             n, a, lda, wr.data, wi.data, vl, ldvl, vr.data, ldvr, 
             work, lwork, info, 1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dgeev", info );
    }

    for(int i=0;i<n;i++) {
        values[i] = std::complex<double>(wr.data[i], wi.data[i] );
    }

    /*
    ** LAPACK returns the eigen vectors as complex conjuate pairs 
    ** if the eigen value is real  ( imaginary part == 0 ) then the eigen vector is real
    ** else the vectors are returned with the real part in the jth column and the
    ** imaginary part in the j+1 column
    */
    for(int j=0;j<n;j++) {
        if( fabs(wi.data[j]) < EPS ) {
            for(int i=0;i<n;i++) {
                rightVectors[j*n+i] = std::complex<double>(vr.data[j*n+i], 0.0 );
             }
        } else {
            for(int i=0;i<n;i++) {
                rightVectors[j*n+i] = std::complex<double>(vr.data[j*n+i], vr.data[(j+1)*n+i]);
                rightVectors[(j+1)*n+i] = std::complex<double>(vr.data[j*n+i], -vr.data[(j+1)*n+i]);
            }
            j++;
        }
    } 
/*
    for(int j=0;j<n;j++) { 
        for(int i=0;i<n;i++) printf("%f %f    ", rightVectors(i,j).real(), rightVectors(i,j).imag() ); 
        printf("\n");
    }
*/
#endif
}

template <> void LapackInterface::geev<float>
   (char jobvl, char jobvr,
    int n, float* a, int lda, std::complex<float>* values,
    float* vl, int ldvl, std::complex<float>* rightVectors, int ldvr, float* work,
    int lwork, int& info )
{
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geev called"));
#else SIMBODY_WITHOUT_LAPACK
    TypedWorkSpace<float> wr(n);
    TypedWorkSpace<float> wi(n);
    TypedWorkSpace<float> vr(n*n);

    // avoid valgrind uninitialized warnings
    for(int i=0;i<n;i++) wi.data[i] = 0;  

    sgeev_( jobvl, jobvr, 
             n, a, lda, wr.data, wi.data, vl, ldvl, vr.data, ldvr, 
             work, lwork, info, 
             1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "sgeev", info );
    }

    for(int i=0;i<n;i++) {
        values[i] = std::complex<float>(wr.data[i], wi.data[i] );
    }
    /*
    ** LAPACK returns the eigen vectors as complex conjuate pairs 
    ** if the eigen value is real  ( imaginary part == 0 ) then the eigen vector is real
    ** else the vectors are returned with the real part in the jth column and the
    ** imaginary part in the j+1 column
    */
    for(int j=0;j<n;j++) {
        if( fabs(wi.data[j]) < (float)EPS ) {
            for(int i=0;i<n;i++) {
                rightVectors[j*n+i] = std::complex<float>(vr.data[j*n+i], 0.0 );
//printf(" %f ",vr.data[j*n+i] );
             }
//printf("\n");
        } else {
            for(int i=0;i<n;i++) {
                rightVectors[j*n+i] = std::complex<float>(vr.data[j*n+i], vr.data[(j+1)*n+i]);
                rightVectors[(j+1)*n+i] = std::complex<float>(vr.data[j*n+i], -vr.data[(j+1)*n+i]);
            }
            j++;
    }
    }
#endif
}

template <> void LapackInterface::geev<std::complex<float> >
   (char jobvl, char jobvr,
    int n, std::complex<float>* a, int lda, std::complex<float>* values, 
    std::complex<float>* vl, int ldvl, std::complex<float>* rightVectors, int ldvr, std::complex<float>* work,
    int lwork, int& info )
{
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geev called"));
#else SIMBODY_WITHOUT_LAPACK
    TypedWorkSpace<float> Rwork(2*n);
    cgeev_( jobvl, jobvr, 
             n, a, lda, values,  vl, ldvl, rightVectors, ldvr, 
             work, lwork, Rwork.data, info, 
             1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cgeev", info );
    }
#endif
}

template <> void LapackInterface::geev<std::complex<double> >
   (char jobvl, char jobvr,
    int n, std::complex<double>* a, int lda, std::complex<double>* values, 
    std::complex<double>* vl, int ldvl, std::complex<double>* rightVectors, int ldvr, std::complex<double>* work,
    int lwork, int& info )
{
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geev called"));
#else SIMBODY_WITHOUT_LAPACK
    TypedWorkSpace<double> Rwork(2*n);
    zgeev_( jobvl, jobvr, 
             n, a, lda, values,  vl, ldvl, rightVectors, ldvr, 
             work, lwork, Rwork.data, info, 
             1, 1);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zgeev", info );
    }
#endif
}

template <> 
void  LapackInterface::getrf<double>( const int m, const int n, double *lu, const int lda,  int *pivots, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrf called"));
#else SIMBODY_WITHOUT_LAPACK
    dgetrf_(m, n, lu, lda, pivots, info   );
#endif
   return;
}
template <> 
void  LapackInterface::getrf<float>( const int m, const int n, float *lu, const int lda,  int *pivots, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrf called"));
#else SIMBODY_WITHOUT_LAPACK
    sgetrf_(m, n, lu, lda, pivots, info   );
#endif
   return;
}

template <> 
void  LapackInterface::getrf<std::complex<double> >( const int m, const int n, std::complex<double> *lu, const int lda,  int *pivots, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrf called"));
#else SIMBODY_WITHOUT_LAPACK
    zgetrf_(m, n, lu, lda, pivots, info   );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zgetrf", info );
    }
#endif
   return;
}

template <> 
void  LapackInterface::getrf<std::complex<float> >( const int m, const int n, std::complex<float> *lu, const int lda,  int *pivots, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getrf called"));
#else SIMBODY_WITHOUT_LAPACK
    cgetrf_(m, n, lu, lda, pivots, info   );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cgetrf", info );
    }
#endif
   return;
}

template <> 
void LapackInterface::tzrzf<double>( const int& m, const int& n,  double* a, const int& lda, double* tau, double* work, const int& lwork, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::tzrzf called"));
#else SIMBODY_WITHOUT_LAPACK
    dtzrzf_(m, n, a, lda, tau, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dtzrzf", info );
    }
#endif
    return;
}

template <> 
void LapackInterface::tzrzf<float>( const int& m, const int& n,  float* a, const int& lda, float* tau, float* work, const int& lwork, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::tzrzf called"));
#else SIMBODY_WITHOUT_LAPACK    
    stzrzf_(m, n, a, lda, tau, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "stzrzf", info );
    }
#endif
    return;
}

template <> 
void LapackInterface::tzrzf<std::complex<double> >( const int& m, const int& n,  std::complex<double>* a, const int& lda, std::complex<double>* tau, std::complex<double>* work, const int& lwork, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::tzrzf called"));
#else SIMBODY_WITHOUT_LAPACK      
    ztzrzf_(m, n, a, lda, tau, work, lwork, info );
#endif
    return;
}

template <> 
void LapackInterface::tzrzf<std::complex<float> >( const int& m, const int& n,  std::complex<float>* a, const int& lda, std::complex<float>* tau, std::complex<float>* work, const int& lwork, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::tzrzf called"));
#else SIMBODY_WITHOUT_LAPACK      
    ctzrzf_(m, n, a, lda, tau, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "ctzrzf", info );
    }
#endif
    return;
}

template <> 
void LapackInterface::geqp3<double>( const int& m, const int& n,  double* a, const int& lda, int *pivots, double* tau, double* work, const int& lwork, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geqp3 called"));
#else SIMBODY_WITHOUT_LAPACK       
     dgeqp3_( m, n, a, lda, pivots, tau, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dgeqp3", info );
    }
#endif
     return;
}

template <> 
void LapackInterface::geqp3<float>( const int& m, const int& n,  float* a, const int& lda, int *pivots, float* tau, float* work, const int& lwork, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geqp3 called"));
#else SIMBODY_WITHOUT_LAPACK        
     sgeqp3_( m, n, a, lda, pivots, tau, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "sgeqp3", info );
    }
#endif
     return;
}

template <> 
void LapackInterface::geqp3<std::complex<float> >( const int& m, const int& n,  std::complex<float>* a, const int& lda, int *pivots, std::complex<float>* tau, std::complex<float>* work, const int& lwork,  int& info ) {
     TypedWorkSpace<float> rwork(2*n);
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geqp3 called"));
#else SIMBODY_WITHOUT_LAPACK         
     cgeqp3_( m, n, a, lda, pivots, tau, work, lwork, rwork.data, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cgeqp3", info );
    }
#endif
     return;
}

template <> 
void LapackInterface::geqp3<std::complex<double> >( const int& m, const int& n,  std::complex<double>* a, const int& lda, int *pivots, std::complex<double>*  tau, std::complex<double>* work, const int& lwork,  int& info ) {
     TypedWorkSpace<double> rwork(2*n);
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::geqp3 called"));
#else SIMBODY_WITHOUT_LAPACK       
     zgeqp3_( m, n, a, lda, pivots, tau, work,  lwork, rwork.data, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zgeqp3", info );
    }
#endif
     return;
}

template <> 
void LapackInterface::lascl<double>( const char& type, const int& kl, const int& ku, const double& cfrom, const double& cto,  const int& m, const int& n, double* a, const int& lda, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lascl called"));
#else SIMBODY_WITHOUT_LAPACK  
//TODO     dlascl_( type, kl, ku, cfrom, cto, m, n, a, lda, info, 1 ); 
    dlascl_( type, kl, ku, &cfrom, &cto, m, n, a, lda, info, 1 ); 

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dlascl", info );
    }
#endif
    return;
}

template <> 
void LapackInterface::lascl<float>( const char& type, const int& kl, const int& ku, const float& cfrom, const float& cto,  const int& m, const int& n, float* a, const int& lda, int& info ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lascl called"));
#else SIMBODY_WITHOUT_LAPACK  
// TODO    slascl_( type, kl, ku, cfrom, cto, m, n, a, lda, info, 1 ); 
    slascl_( type, kl, ku, &cfrom, &cto, m, n, a, lda, info, 1 ); 

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "slascl", info );
    }
#endif
    return;
}

template <> 
void LapackInterface::lascl<std::complex<float> >( const char& type, const int& kl, const int& ku, const float& cfrom, const float& cto,  const int& m, const int& n, std::complex<float>* a, const int& lda, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lascl called"));
#else SIMBODY_WITHOUT_LAPACK 
// TODO    clascl_( type, kl, ku, cfrom, cto, m, n, a, lda, info, 1 ); 
    clascl_( type, kl, ku, &cfrom, &cto, m, n, a, lda, info, 1 ); 

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "clascl", info );
    }
#endif
    return;
}

template <> 
void LapackInterface::lascl<std::complex<double> >( const char& type, const int& kl, const int& ku, const double& cfrom, const double& cto,  const int& m, const int& n, std::complex<double>* a, const int& lda, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lascl called"));
#else SIMBODY_WITHOUT_LAPACK 
// TODO    zlascl_( type, kl, ku, cfrom, cto, m, n, a, lda, info, 1 ); 
    zlascl_( type, kl, ku, &cfrom, &cto, m, n, a, lda, info, 1 ); 

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zlascl", info );
    }
#endif
    return;
}


template <> 
double LapackInterface::lange<float>( const char& norm, const int& m, const int& n, const float* a, const int& lda){
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lange called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK 
/*
 TODO JACKM because g77 returns FORTRAN REAL's as doubles and gfortran returns them as floats
 changes this once everyone has changed to new libraries and SimTKlapack.h has been updated
     TypedWorkSpace<float> work(m);
     return( slange_( norm, m, n, a, lda, work.data, 1 ) ); 
*/

     TypedWorkSpace<double> work(m);
     TypedWorkSpace<double> da(m*n);
     // Copy float matrix in Lapack full storage format into temporary
     // dense double matrix.
     for (int j=0; j<n; j++)
         for (int i=0; i<m; i++)
             da.data[j*m + i] = a[j*lda + i];
     // leading dimension of da.data is m now, not lda
     return( dlange_( norm, m, n, da.data, m, work.data, 1 ) );
#endif
}
 
template <> 
double LapackInterface::lange<double>( const char& norm, const int& m, const int& n, const double* a, const int& lda ){
     TypedWorkSpace<double> work(m);
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lange called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK 
     return( dlange_( norm, m, n, a, lda, work.data, 1 ) ); 
#endif
}
 
template <> 
double LapackInterface::lange<std::complex<float> >( const char& norm, const int& m, const int& n, const std::complex<float>* a, const int& lda ){
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lange called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK 
/*
 TODO JACKM because g77 returns FORTRAN REAL's as doubles and gfortran returns them as floats
 switch to correct LAPACK call when everyone uses SimTKlapack.h has been updated
     TypedWorkSpace<float> work(m);
     return( clange_( norm, m, n, a, lda, work.data, 1 ) );
*/
     TypedWorkSpace<double> work(m);
     TypedWorkSpace<std::complex<double> > za(m*n);

     // Copy float matrix in Lapack full storage format into temporary
     // dense double matrix.
     for (int j=0; j<n; j++)
         for (int i=0; i<m; i++)
             za.data[j*m + i] = a[j*lda + i];
     // leading dimension of za.data is m now, not lda
     std::cout << "LapackInterface::lange called" << std::endl;
     return zlange_( norm, m, n, za.data, m, work.data, 1 );
#endif    
}
 
template <> 
double LapackInterface::lange<std::complex<double> >( const char& norm, const int& m, const int& n, const std::complex<double>* a, const int& lda) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::lange called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK 
     TypedWorkSpace<double> work(m);
     return( zlange_( norm, m, n, a, lda, work.data, 1 ) );
#endif  
}
 
template <> 
void LapackInterface::ormqr<float>(const char& side, const char& trans, const int& m, const int& n, const int& k, float* a, const int& lda, float *tau, float *c__, const int& ldc, float* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormqr called"));
#else SIMBODY_WITHOUT_LAPACK 
     sormqr_( side, trans, m, n, k, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "sormqr", info );
    }
#endif 
     return;
}

template <> 
void LapackInterface::ormqr<double>(const char& side, const char& trans, const int& m, const int& n, const int& k, double* a, const int& lda, double *tau, double *c__, const int& ldc, double* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormqr called"));
#else SIMBODY_WITHOUT_LAPACK 
     dormqr_( side, trans, m, n, k, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dormqr", info );
    }
#endif
     return;
}

template <> 
void LapackInterface::ormqr<std::complex<double> >(const char& side, const char& trans, const int& m, const int& n, const int& k, std::complex<double>* a, const int& lda, std::complex<double> *tau, std::complex<double> *c__, const int& ldc, std::complex<double>* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormqr called"));
#else SIMBODY_WITHOUT_LAPACK 
     zunmqr_( side, trans, m, n, k, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zunmqr", info );
    }
#endif
     return;
}

template <> 
void LapackInterface::ormqr<std::complex<float> >(const char& side, const char& trans, const int& m, const int& n, const int& k, std::complex<float>* a, const int& lda, std::complex<float> *tau, std::complex<float> *c__, const int& ldc, std::complex<float>* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormqr called"));
#else SIMBODY_WITHOUT_LAPACK 
     cunmqr_( side, trans, m, n, k, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cunmqr", info );
    }
#endif
     return;
}

template <> 
void LapackInterface::trsm<float>(const char& side, const char& uplo, const char& transA, const char& diag, const int& m, const int& n, const float& alpha, const float* a, const int& lda, float* b, const int& ldb ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::trsm called"));
#else SIMBODY_WITHOUT_LAPACK 
     strsm_( side, uplo, transA, diag, m, n, alpha, a, lda, b, ldb, 1, 1, 1 );
#endif
     return;
}

template <> 
void LapackInterface::trsm<double>(const char& side, const char& uplo, const char& transA, const char& diag, const int& m, const int& n, const double& alpha, const double* a, const int& lda, double* b, const int& ldb ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::trsm called"));
#else SIMBODY_WITHOUT_LAPACK 
     dtrsm_( side, uplo, transA, diag, m, n, alpha, a, lda, b, ldb, 1, 1, 1 );
#endif
     return;
}

template <> 
void LapackInterface::trsm<std::complex<double> >(const char& side, const char& uplo, const char& transA, const char& diag, const int& m, const int& n, const std::complex<double>& alpha, const std::complex<double>* a, const int& lda, std::complex<double>* b, const int& ldb ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::trsm called"));
#else SIMBODY_WITHOUT_LAPACK 
     ztrsm_( side, uplo, transA, diag, m, n, alpha, a, lda, b, ldb, 1, 1, 1 );
#endif
     return;
}

template <> 
void LapackInterface::trsm<std::complex<float> >(const char& side, const char& uplo, const char& transA, const char& diag, const int& m, const int& n, const std::complex<float>& alpha, const std::complex<float>* a, const int& lda, std::complex<float>* b, const int& ldb ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::trsm called"));
#else SIMBODY_WITHOUT_LAPACK 
     ctrsm_( side, uplo, transA, diag, m, n, alpha, a, lda, b, ldb, 1, 1, 1 );
#endif
     return;
}

template <> 
void LapackInterface::ormrz<float>(const char& side, const char& trans, const int& m, const int& n, const int& k, const int& l, float* a, const int& lda, float* tau, float* c__, const int& ldc, float* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormrz called"));
#else SIMBODY_WITHOUT_LAPACK 
   sormrz_( side, trans, m, n, k, l, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "sormrz", info );
    }
#endif
   return;
}

template <> 
void LapackInterface::ormrz<double>(const char& side, const char& trans, const int& m, const int& n, const int& k, const int& l, double* a, const int& lda, double* tau, double* c__, const int& ldc, double* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormrz called"));
#else SIMBODY_WITHOUT_LAPACK 
   dormrz_( side, trans, m, n, k, l, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dormrz", info );
    }
#endif
   return;
}

template <> 
void LapackInterface::ormrz<std::complex<float> >(const char& side, const char& trans, const int& m, const int& n, const int& k, const int& l, std::complex<float>* a, const int& lda, std::complex<float>* tau, std::complex<float>* c__, const int& ldc, std::complex<float>* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormrz called"));
#else SIMBODY_WITHOUT_LAPACK 
   cunmrz_( side, trans, m, n, k, l, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cunmrz", info );
    }
#endif
   return;
}

template <> 
void LapackInterface::ormrz<std::complex<double> >(const char& side, const char& trans, const int& m, const int& n, const int& k, const int& l, std::complex<double>* a, const int& lda, std::complex<double>* tau, std::complex<double>* c__, const int& ldc, std::complex<double>* work, const int& lwork, int& info) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ormrz called"));
#else SIMBODY_WITHOUT_LAPACK 
   zunmrz_( side, trans, m, n, k, l, a, lda, tau, c__, ldc, work, lwork, info, 1, 1 );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zunmrz", info );
    }
#endif
   return;
}

template <> 
void LapackInterface::copy<float>( const int& n, const float* x, const int& incx, float* y, const int& incy) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::copy called"));
#else SIMBODY_WITHOUT_LAPACK 
     scopy_(n, x, incx, y, incy );
#endif
     return;
}

template <> 
void LapackInterface::copy<double>( const int& n, const double* x, const int& incx, double* y, const int& incy) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::copy called"));
#else SIMBODY_WITHOUT_LAPACK 
     dcopy_(n, x, incx, y, incy );
#endif
     return;
}

template <> 
void LapackInterface::copy<std::complex<float> >( const int& n, const std::complex<float>* x, const int& incx, std::complex<float>* y, const int& incy) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::copy called"));
#else SIMBODY_WITHOUT_LAPACK 
     ccopy_(n, x, incx, y, incy );
#endif
     return;
}

template <> 
void LapackInterface::copy<std::complex<double> >( const int& n, const std::complex<double>* x, const int& incx, std::complex<double>* y, const int& incy) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::copy called"));
#else SIMBODY_WITHOUT_LAPACK 
     zcopy_(n, x, incx, y, incy );
#endif
     return;
}
template <>
void LapackInterface::getMachineUnderflow<float>( float& underFlow ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getMachineUnderflow called"));
#else SIMBODY_WITHOUT_LAPACK 
    underFlow = slamch_('S');
#endif
    return;
}
template <>
void LapackInterface::getMachineUnderflow<double>( double& underFlow ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getMachineUnderflow called"));
#else SIMBODY_WITHOUT_LAPACK 
    underFlow = dlamch_('S');
#endif
    return;
}
template <>
void LapackInterface::getMachinePrecision<float>( float& smallNumber, float& bigNumber ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getMachinePrecision called"));
#else SIMBODY_WITHOUT_LAPACK     
    smallNumber = slamch_( 'S' )/slamch_( 'P' );
    bigNumber = 1.f/smallNumber;
    slabad_(smallNumber, bigNumber );
#endif
}

template <>
void LapackInterface::getMachinePrecision<double>( double& smallNumber, double& bigNumber ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::getMachinePrecision called"));
#else SIMBODY_WITHOUT_LAPACK     
    smallNumber = dlamch_( 'S' )/dlamch_( 'P' );
    bigNumber = 1.0/smallNumber;
    dlabad_(smallNumber, bigNumber );
#endif
}

template <> 
void LapackInterface::laic1<float>(const int& job, const int& j, const float* x, const float& sest, const float* w, const float& gamma, float& sestpr, float& s, float& c__ ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::laic1 called"));
#else SIMBODY_WITHOUT_LAPACK   
    slaic1_( job, j, x, sest, w, gamma, sestpr, s, c__ );
#endif
    return;
}

template <> 
void LapackInterface::laic1<double>(const int& job, const int& j, const double* x, const double& sest, const double* w, const double& gamma, double& sestpr, double& s, double& c__ ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::laic1 called"));
#else SIMBODY_WITHOUT_LAPACK  
    dlaic1_( job, j, x, sest, w, gamma, sestpr, s, c__ );
#endif
    return;
}

template <> 
void LapackInterface::laic1<std::complex<float> >(const int& job, const int& j, const std::complex<float>* x, const float& sest, const std::complex<float>* w, const std::complex<float>& gamma, float& sestpr, std::complex<float>& s, std::complex<float>& c__ ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::laic1 called"));
#else SIMBODY_WITHOUT_LAPACK  
    claic1_( job, j, x, sest, w, gamma, sestpr, s, c__ );
#endif
    return;
}


template <> 
void LapackInterface::laic1<std::complex<double> >(const int& job, const int& j, const std::complex<double>* x, const double& sest, const std::complex<double>* w, const std::complex<double>& gamma, double& sestpr, std::complex<double>& s, std::complex<double>& c__ ) {
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::laic1 called"));
#else SIMBODY_WITHOUT_LAPACK  
    zlaic1_( job, j, x, sest, w, gamma, sestpr, s, c__ );
#endif
    return;
}

template <>
void LapackInterface::potrf<double>( const char& uplo, const int n,  double* a, const int lda, int& info ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    dpotrf_(uplo, n, a, lda, info);
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dpotrf", info );
    }
#endif

    return;
 }
template <>
void LapackInterface::potrf<float>( const char& uplo, const int n,  float* a, const int lda, int& info ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    spotrf_(uplo, n, a, lda, info);
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "spotrf", info );
    }
#endif

    return;
 }
template <>
void LapackInterface::potrf<std::complex<double> >( const char& uplo, const int n,  std::complex<double>* a, const int lda, int& info ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    zpotrf_(uplo, n, a, lda, info);
    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zpotrf", info );
    }
#endif

    return;
 }
template <>
void LapackInterface::potrf<std::complex<float> >( const char& uplo, const int n,  std::complex<float>* a, const int lda, int& info ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::potrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    cpotrf_(uplo, n, a, lda, info);

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "cpotrf", info );
    }
#endif

    return;
 }
template <> 
void LapackInterface::sytrf<float>( const char& uplo, const int n, float* a,  const int lda, int* pivots, float* work, const int lwork, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    ssytrf_( uplo, n, a, lda, pivots, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "ssytrf", info );
    }
#endif
    return;
}
template <> 
void LapackInterface::sytrf<double>( const char& uplo, const int n, double* a,  const int lda, int* pivots, double* work, const int lwork, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    dsytrf_( uplo, n, a, lda, pivots, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "dsytrf", info );
    }
#endif
    return;
}
template <> 
void LapackInterface::sytrf<std::complex<double> >( const char& uplo, const int n, std::complex<double>* a,  const int lda, int* pivots, std::complex<double>* work, const int lwork, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    zsytrf_( uplo, n, a, lda, pivots, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "zsytrf", info );
    }
#endif
    return;
}
template <> 
void LapackInterface::sytrf<std::complex<float> >( const char& uplo, const int n, std::complex<float>* a,  const int lda, int* pivots, std::complex<float>* work, const int lwork, int& info){ 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::sytrf called"));
#else SIMBODY_WITHOUT_LAPACK  
    csytrf_( uplo, n, a, lda, pivots, work, lwork, info );

    if( info < 0 ) {
        SimTK_THROW2( SimTK::Exception::IllegalLapackArg, "csytrf", info );
    }
#endif
    return;
}
template <typename T> 
void LapackInterface::sytrf( const char& uplo, const int n, T* a,  const int lda, int *pivots, T* work, const int lwork, int& info ) { assert(false); }

template <>
int LapackInterface::ilaenv<double>( const int& ispec,  const char* name,  const char *opts, const int& n1, const int& n2, const int& n3, const int& n4 ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ilaenv called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK  
     char d[10];
     d[0] = 'd';
     d[1] = '\0';
     return (ilaenv_( ispec, strcat( d, name), opts, n1, n2, n3, n3, 6, (int)strlen(opts)) );
#endif 
}
template <>
int LapackInterface::ilaenv<float>( const int& ispec,  const char* name,  const char *opts, const int& n1, const int& n2, const int& n3, const int& n4 ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ilaenv called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK  
     char s[10];
     s[0] = 's';
     s[1] = '\0';
     return (ilaenv_( ispec, strcat( s, name), opts, n1, n2, n3, n3, 6, (int)strlen(opts)) ); 
#endif 
}
template <>
int LapackInterface::ilaenv<std::complex<double> >( const int& ispec,  const char* name,  const char *opts, const int& n1, const int& n2, const int& n3, const int& n4 ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ilaenv called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK  
    char z[10];
     z[0] = 'z';
     z[1] = '\0';
     return (ilaenv_( ispec, strcat( z, name), opts, n1, n2, n3, n3, 6, (int)strlen(opts)) ); 
#endif 
}
template <>
int LapackInterface::ilaenv<std::complex<float> >( const int& ispec,  const char* name,  const char *opts, const int& n1, const int& n2, const int& n3, const int& n4 ) { 
#ifdef SIMBODY_WITHOUT_LAPACK
    throw std::runtime_error(std::string("LapackInterface::ilaenv called"));
    return 0;
#else SIMBODY_WITHOUT_LAPACK  
     char c[10];
     c[0] = 'c';
     c[1] = '\0';
     return (ilaenv_( ispec, strcat( c, name), opts, n1, n2, n3, n3, 6, (int)strlen(opts)) ); 
#endif 
}


}   // namespace SimTK

