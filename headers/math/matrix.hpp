#ifndef __SIGABRT_NUMERIC_MATRIX__
#define __SIGABRT_NUMERIC_MATRIX__


#include <cstddef>
#include <exception>
#include <memory>
#include <vector>

#include <models.hpp>

using Sigabrt::Numeric::Models::Slice;

namespace Sigabrt {
    namespace Numeric {
        /**
         * \class Matrix
         * \tparam T: Some numeric type
         * 
         * \brief Matrix class
         * 
         * Matrix stricture. This represents a 2D matrix of elements of type `T` with some constrains. This structure has the copy
         * constructor and assignment operators deleted to prevent extremely expensive operations.
         * 
         * This structure defines methods for basic row operations:
         *   - Linear combination
         *   - Exchange
         *   - Scalar multiplication
         *   - Find next pivot element after a row. This is necessary in algorithms like Gauss Jordan, if the current pivot is 0.
         * NOTE: All row operations throw `out_of_range` error on invalid rows.
         * 
         * This also has the addition and multiplication operators overriden. Multiply operator can also multiply the matrix with a vector.
         * NOTE: When we matrix*vector, we assume it's a column vector and the result is a column vector as well, but when doing vector
         * *matrix, we assume the vector and the result are row vectors.
         *
         * Matrix also has index operations and range for defined. You can do `matrix[i][j]` on both const and non const objects/references,
         * and also use this in range for loops like `for (auto row : matrix)`. For your convenience, this library also defines * operator for 2
         * vectors, the same way as it does for matrices.
         * 
         * */
        template <typename T> class Matrix {
        private:
            std::size_t nrows;
            std::size_t ncols;
            std::unique_ptr<Slice<T>[]> rows;
            std::unique_ptr<T[]> storage;
            
            void initializeSlices() {
                for (std::size_t i = 0; i < nrows; i++) {
                    rows[i].start = storage.get()+i*ncols;
                    rows[i].size = ncols;
                }
            }

        public:
            /**
             * \brief Constructs a nrows x ncols empty matrix.
             * 
             * \param nrows: Number of rows.
             * \param ncols: Number of columns in matrix.
             * 
             * \return Matrix<T> (rows x cols)
             * 
             * */
            Matrix(
                const std::size_t& nrows, 
                const std::size_t& ncols
            ): nrows {nrows}, 
                ncols {ncols}, 
                rows {std::make_unique<Slice<T>[]>(nrows)},
                storage {std::make_unique<T[]>(nrows * ncols)} {
                    initializeSlices();
                }

            /**
             * \brief: Constructs a nrows x ncols matrix from a vector of vectors.
             * 
             * \param vecs: vector of vectors of type `T`
             * 
             * \return Matrix<T>
             * */
            Matrix(const std::vector<std::vector<T>>& vecs) {
                if (vecs.size() == 0) {
                    throw std::invalid_argument("Matrix cannot have 0 rows.");
                } else if (vecs[0].size() == 0) {
                    throw std::invalid_argument("Matrix cannot row with 0 elements.");
                } else {
                    nrows = vecs.size();
                    ncols = vecs[0].size();
                    rows = std::make_unique<Slice<T>[]>(nrows);
                    storage = std::make_unique<T[]>(nrows * ncols);
                    
                    for (std::size_t i = 0; i < nrows; i++) {
                        if (vecs[i].size() != ncols) {
                            throw std::invalid_argument("All rows of source matrix have to be of same length.");
                        }
                        for (std::size_t j = 0; j < ncols; j++) {
                            storage[i*ncols + j] = vecs[i][j];
                        }
                    }
                    initializeSlices();
                }
            }

            Matrix(const Matrix<T>& other)=delete;
            void operator=(const Matrix<T> other)=delete;

            /**
             * \brief: Move constructor
             * 
             * \param other: Matrix<T>&&
             * 
             * \return Matrix<T>
             * 
             * */
            Matrix(Matrix<T>&& other): nrows {other.nrows}, ncols {other.ncols}, rows {std::move(other.rows)}, storage {std::move(other.storage)} {
                other.nrows = 0;
                other.ncols = 0;
                other.rows = nullptr;
                other.storage = nullptr;
            }

            Slice<T>& operator[](const std::size_t& row) {
                if (row >= nrows) {
                    throw std::out_of_range("Array index out of range");
                }
                
                return rows[row];
            }

            const Slice<T>& operator[](const std::size_t& row) const {
                if (row >= nrows) {
                    throw std::out_of_range("Array index out of range");
                }
                return rows[row];
            }

            const Slice<T>* begin() const {
                return &rows[0];
            }

            const Slice<T>* end() const {
                return &rows[nrows];
            }

            Slice<T>* begin() {
                return &rows[0];
            }

            Slice<T>* end() {
                return &rows[nrows];
            }

            /**
             * \fn identity
             * 
             * \brief Identity matrix generator.
             * 
             * This static function generates and returns a NxN identity matrix of type `T`.
             * 
             * \param n: Size of the resulting identity matrix
             * 
             * \return Matrix<T>, whuch is a nxn identity matrix.
             * 
             * */
            static Matrix<T> identity(const std::size_t& sz) {
                Matrix<T> retval {sz, sz};
                for (std::size_t i = 0; i < sz; i++) {
                    retval[i][i] = static_cast<T>(1);
                }
                return retval;
            }

            /**
             * \fn zero
             * 
             * \brief Zero matrix generator
             * 
             * This function generates a Zero matrix of dimensions nrows x ncols.
             * 
             * \param nrows: Rows in the Zero matrix.
             * \param ncols: Cols in the Zero matrix.
             * 
             * \return Matrix<T> (nrows x ncols), with all elements set to 0.
             * */
            static Matrix<T> zero(const std::size_t& rows, const std::size_t& cols) {
                Matrix<T> retval {rows, cols};
                for (auto& row: retval) {
                    for (auto& elem : row) {
                        elem = static_cast<T>(0);
                    }
                }
                return retval;
            }
            
            /**
             * \brief Get the number of rows in the matrix.
             * */
            const std::size_t& getRows() const {
                return nrows;
            }
            
            /**
             * \brief Get the number of columns in the matrix.
             * */
            const std::size_t& getCols() const {
                return ncols;
            }
            
            /**
             * \brief Linear combination of rows.
             * 
             * This function replaces a row with a linear combination of it and another row. R1 -> aR1 + bR2.
             * This function 
             * 
             * \param r1: The row to replace.
             * \param a: The factor to scale R1 by in the linear combination.
             * \param r2: The other row.
             * \param b: The factir to scale R2 by in the linear combination.
             * 
             * \throw e: std::out_of_range if either r1 or r2 exeeds the length of the matrix.
             * */
            void linearCombRows(
                const std::size_t& r1,
                const T& a,
                const std::size_t& r2,
                const T& b
            ) {
                if (r1 > nrows || r2 > nrows) {
                    throw std::out_of_range("Row access out of range.");
                } else {
                    for (std::size_t i = 0; i < ncols; i++) {
                        rows[r1][i] = a*rows[r1][i] + b*rows[r2][i];
                    }
                }
            }
            
            /**
             * \brief Exchange rows
             * 
             * This function exchanges 2 rows in the matrix.
             * 
             * \param r1: First row to exchange.
             * \param r2: Other row to exchange.
             * 
             * \throw e: std::out_of_range if either r1 or r2 exeeds the length of the matrix.
             * */
            void exchangeRows(const std::size_t& r1, const std::size_t& r2) {
                if (r1 > nrows || r2 > nrows) {
                    throw std::out_of_range("Row access out of range.");
                } else {
                    std::swap(rows[r1].start, rows[r2].start);
                }
            }

            /**
             * \brief Scale a row.
             * 
             * This function scales a row by a factor.
             * 
             * \param row: The row to scale.
             * \param factor: The factor to scale the row by.
             * 
             * \throw e: std::out_of_range if either r1 or r2 exeeds the length of the matrix.
             * */
            void scale(const std::size_t& row, const T& factor) {
                if (row > nrows) {
                    throw std::out_of_range("Row access out of range.");
                }
                for (auto& elem : rows[row]) {
                    elem *= factor;
                }
            }
        };
        
        // Override add operator
        template <typename T> Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
            if (lhs.getRows() != rhs.getRows() || lhs.getCols() != rhs.getCols()) {
                throw std::invalid_argument("Matrices of different dimensions cannot be added");
            }
            Matrix<T> retval {lhs.getRows(), lhs.getCols()};
            for (std::size_t i = 0; i < lhs.getRows(); i++) {
                for (std::size_t j = 0; j < lhs.getCols(); j++) {
                    retval[i][j] = lhs[i][j] + rhs[i][j];
                }
            }
            return retval;
        }
        
        // Override multiply operator.
        template <typename T> Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
            if (lhs.getCols() != rhs.getRows()) {
                throw std::invalid_argument("Incompatible matrices for multiplication.");
            }
            Matrix<T> retval {lhs.getRows(), rhs.getCols()};
            
            for (std::size_t i = 0; i < lhs.getRows(); i++) {
                
                for (std::size_t j = 0; j < rhs.getCols(); j++) {
                    
                    T sum = static_cast<T>(0);
                    for (std::size_t k = 0; k < lhs.getCols(); k++) {
                            sum += lhs[i][k] * rhs[k][j];
                    }
                    retval[i][j] = sum;
                }
            }
            return retval;
        }
        
        // Override multiply operator  lhs = matrix and rhs = vector.
        template <typename T> std::vector<T> operator*(const Matrix<T>& lhs, const std::vector<T>& rhs) {
            if (lhs.getCols() != rhs.size()) {
                throw std::invalid_argument("Incompatible matrix and vector for multiplication.");
            }
            
            std::vector<T> retval;
            retval.reserve(lhs.getRows());
            
            for (std::size_t i = 0; i < lhs.getRows(); i++) {
                T sum = static_cast<T>(0);
                for (std::size_t k = 0; k < lhs.getCols(); k++) {
                    sum += lhs[i][k] * rhs[k];
                }
                retval.push_back(sum);
            }
            
            return retval;
        }
        
        // Override multiply operator lhs = vector and rhs = matrix.
        template <typename T> std::vector<T> operator*(const std::vector<T>& lhs, const Matrix<T>& rhs) {
            return rhs*lhs;
        }
        
    }
}

template <typename T> T operator*(const std::vector<T>& v1, const std::vector<T>& v2) {
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Incompatible vectors for multiplication.");
    }
    T acc {static_cast<T>(0)};
    for (auto iter1 = v1.begin(), iter2 = v2.begin(); iter1 < v1.end(); iter1++, iter2++) {
        acc+= ((*iter1) * (*iter2));
    }
    return acc;
}

#endif
