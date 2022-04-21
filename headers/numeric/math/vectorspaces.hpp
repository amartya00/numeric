#ifndef __SIGABRT_NUMERIC_VECTORSPACES__
#define __SIGABRT_NUMERIC_VECTORSPACES__

#include <cmath>
#include <functional>
#include <vector>

#include <numeric/types/vector.hpp>
#include <numeric/types/plane.hpp>
#include <numeric/types/models.hpp>
#include <numeric/types/matrix.hpp>

#include <numeric/math/rref.hpp>
#include <numeric/math/errors.hpp>


/**
 * \namespace numeric
 * 
 * \brief The root namespace.
 * */
namespace numeric {
    /**
     * \namespace numeric::functions
     * 
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace functions {

        /**
         * \brief Function to test linear independence of 2 vectors.
         * 
         * \tparam T The type of a vector.
         * 
         * \tparam U The type of the other vector.
         * 
         * This function tests linear dependence of 2 vectors using Cauchy-Schwartz inequality. Note that the 2 vectors do not 
         * have to be of the same type, as the magnitude is always computed as double.
         * 
         * Also, it is an error to try and test linear dependence of 2 vectors of unequal dimensions.
         * 
         * \param v1 One of the vectors.
         * 
         * \param v2 The other vector.
         * 
         * \return Sigabrt::Types::Result<bool, Sigabrt::Numeric::ErrorCode> 
         * */
        template<typename T, typename U>
        thesoup::types::Result<bool, numeric::ErrorCode>
        are_linearly_dependent(const numeric::types::Vector<T> &v1, const numeric::types::Vector<U> &v2) {
            if (v1.size() == v2.size()) {
                double dot{static_cast<double>(v1 * v2)};
                return thesoup::types::Result<bool, numeric::ErrorCode>::success({dot * dot == v1.mod() * v2.mod()});
            }
            return thesoup::types::Result<bool, numeric::ErrorCode>::failure(numeric::ErrorCode::INCOMPATIBLE_VECTORS);
        }

        /**
         * \brief Function to compute cosine of angle between 2 vectors.
         * 
         * \tparam T The type of a vector.
         * 
         * \tparam U The type of the other vector.
         * 
         * This function computes the cosine of the angle between 2 vectors, using dot product.
         * 
         * Also, it is an error to and do this with 2 vectors of unequal dimensions.
         * 
         * \param v1 One of the vectors.
         * 
         * \param v2 The other vector.
         * 
         * \return Sigabrt::Types::Result<double, Sigabrt::Numeric::ErrorCode> 
         * */
        template<typename T, typename U>
        thesoup::types::Result<double, numeric::ErrorCode>
        cosine_angle(const numeric::types::Vector<T> &v1, const numeric::types::Vector<U> &v2) {
            if (v1.size() == v2.size()) {
                double dot{static_cast<double>(v1 * v2)};
                return thesoup::types::Result<double, numeric::ErrorCode>::success(
                        {static_cast<double>(dot) / std::sqrt(v1.mod() * v2.mod())});
            }
            return thesoup::types::Result<double, numeric::ErrorCode>::failure(
                    numeric::ErrorCode::INCOMPATIBLE_VECTORS);
        }

        /**
         * \brief Function to check if vector is normal to plane.
         * 
         * \tparam T The type of a vector.
         * 
         * \tparam U The type of the other vector.
         * 
         * This function checks if a Sigabrt::Types::Vector is normal to a Sigabrt::Types::Plane . Note that the type of 
         * the vector and the plane do not have to be the same, just like the cosine method above.
         * 
         * It is an error to try and do this with a vector of non-3 dimensions.
         * 
         * \param v1 One of the vectors.
         * 
         * \param v2 The other vector.
         * 
         * \return Sigabrt::Types::Result<bool, Sigabrt::Numeric::ErrorCode>
         * */
        template<typename T, typename U>
        thesoup::types::Result<bool, numeric::ErrorCode> is_normal_to_plane(
                const numeric::types::Plane<T> &plane,
                const numeric::types::Vector<U> &vec
        ) {
            if (vec.size() == 3) {
                const auto &normal = plane.get_normal();
                double dot{static_cast<double>(normal * vec)};
                double angle{static_cast<double>(dot) / std::sqrt(normal.mod() * vec.mod())};
                return thesoup::types::Result<bool, numeric::ErrorCode>::success({angle == 1.0});
            }
            return thesoup::types::Result<bool, numeric::ErrorCode>::failure(numeric::ErrorCode::INCOMPATIBLE_VECTORS);
        }

        /**
         * \brief Function to compute cross product.
         * 
         * \tparam T The type of a vector.
         * 
         * \tparam U The type of the other vector.
         * 
         * This function computes the cross product between 2 Sigabrt::Types::Vector. Note that the type of 
         * the 2 vectors do not have to be the same, just like the other methods above.
         * 
         * It is an error to try and do this with vectors of non-3 dimensions.
         * 
         * \param v1 One of the vectors.
         * 
         * \param v2 The other vector.
         * 
         * \return Sigabrt::Types::Result<bool, Sigabrt::Numeric::ErrorCode>
         * */
        template<typename T, typename U>
        thesoup::types::Result<numeric::types::Vector<T>, numeric::ErrorCode> cross(
                const numeric::types::Vector<T> &v1,
                const numeric::types::Vector<U> &v2
        ) {
            if (v1.size() == 3 && v2.size() == 3) {
                const T &a1{v1[0]};
                const T &a2{v1[1]};
                const T &a3{v1[2]};

                const T &b1{v2[0]};
                const T &b2{v2[1]};
                const T &b3{v2[2]};

                numeric::types::Vector<T> retval{3};
                retval[0] = static_cast<T>(a2 * b3 - a3 * b2);
                retval[1] = static_cast<T>(a3 * b1 - a1 * b3);
                retval[2] = static_cast<T>(a1 * b2 - a2 * b1);

                return thesoup::types::Result<numeric::types::Vector<T>, numeric::ErrorCode>::success(
                        std::move(retval));
            }
            return thesoup::types::Result<numeric::types::Vector<T>, numeric::ErrorCode>::failure(
                    numeric::ErrorCode::INCOMPATIBLE_VECTORS);
        }


        /**
         * \brief Function to test linear indenendence of a ser of vectors.
         * 
         * \tparam T the type of the vectors.
         * 
         * This function takes a list of `Sigabrt::Types::Vector`s and tests them for linear independence, using matrix null 
         * space methodology. The function returns an instance of `Sigabrt::Types::Result<bool, Sigabrt::Numeric::ErrorCode>` 
         * 
         * The result is true when the vectors ARE independent, otherwise false.
         * 
         * The result is an error if:
         *   - The vectors are of different dimensions. (ErrorCode = `Sigabrt::Types::INCOMPATIBLE_VECTORS`)
         *   - If some other error happend (ErrorCode = `Sigabrt::Types::UNKNOWN_ERROR`)
         * 
         * \param vectors A `std::vector` of `Sigabrt::Types::vector<T>`s
         * 
         * \return Sigabrt::Types::Result<bool, Sigabrt::Numeric::ErrorCode>
         * 
         **/
        template<typename T>
        thesoup::types::Result<bool, numeric::ErrorCode>
        linear_independence_of_system(const std::vector<std::reference_wrapper<numeric::types::Vector<T>>> &vectors) {
            // Linear independence of just one vector does not make any sense
            if (vectors.size() == 1) {
                return thesoup::types::Result<bool, numeric::ErrorCode>::failure(
                        numeric::ErrorCode::UNDERDETERMINED_SYSTEM);
            }

            // If all vectors do not have same dimensions, return an error.
            std::size_t len{vectors[0].get().size()};
            for (const auto &v: vectors) {
                if (v.get().size() != len) {
                    return thesoup::types::Result<bool, numeric::ErrorCode>::failure(
                            numeric::ErrorCode::INCOMPATIBLE_VECTORS);
                }
            }

            // If the matrix is "wider", it cannot be linearly independent.
            if (vectors.size() > len) {
                return thesoup::types::Result<bool, numeric::ErrorCode>::success(false);
            }

            // Initialize the matrix
            numeric::types::Matrix<T> mat{len, vectors.size()};
            for (std::size_t i = 0; i < len; i++) {
                for (std::size_t j = 0; j < vectors.size(); j++) {
                    mat[i][j] = vectors[j].get()[i];
                }
            }

            // Run it through RREF
            thesoup::types::Result<thesoup::types::Unit, numeric::ErrorCode> res{numeric::functions::rref(mat)};

            if (!res) {
                if (res.error() == numeric::ErrorCode::FREE_COLUMNS_RREF) {
                    // A homogenous system has to have solutions. It's either 1 or infinite.
                    // Presence of free variables indicate many solutions, which in turn indicates
                    // linear dependence.
                    return thesoup::types::Result<bool, numeric::ErrorCode>::success(false);
                } else {
                    return thesoup::types::Result<bool, numeric::ErrorCode>::failure(
                            numeric::ErrorCode::UNKNOWN_ERROR);
                }
            } else {
                // No free variables indicates a unique solution which would be V = 0
                return thesoup::types::Result<bool, numeric::ErrorCode>::success(true);
            }
        }
    }
}

#endif
