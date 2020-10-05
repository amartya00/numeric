#ifndef __SIGABRT_NUMERIC_VECTORSPACES__
#define __SIGABRT_NUMERIC_VECTORSPACES__

#include <cmath>
#include <types/vector.hpp>
#include <types/plane.hpp>
#include <types/models.hpp>

/**
 * \namespace Sigabrt
 * 
 * \brief The root namespace.
 * */
namespace Sigabrt {
    /**
     * \namespace Sigabrt::Numeric
     * 
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace Numeric {
        
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
         * \return Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> 
         * */
        template <typename T, typename U> 
        Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> 
        areLinearlyDependent(const Sigabrt::Types::Vector<T>& v1, const Sigabrt::Types::Vector<U>& v2) {
            if (v1.size() == v2.size()){
                double dot {static_cast<double>(v1*v2)};
                bool result {dot*dot == v1.mod()*v2.mod()};
                return Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::OK,
                    result,
                    std::nullopt,
                    std::nullopt
                };
            }
            return Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> {
                Sigabrt::Types::OperationType::ERR,
                std::nullopt,
                Sigabrt::Types::ErrorCode::INCOMPATIBLE_VECTORS,
                std::string("Cannot check linear independence of 2 vectors of unequal dimensions.")
            };
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
         * \return Sigabrt::Types::Result<double, Sigabrt::Types::ErrorCode> 
         * */
        template <typename T, typename U> 
        Sigabrt::Types::Result<double, Sigabrt::Types::ErrorCode> 
        cosineAngle(const Sigabrt::Types::Vector<T>& v1, const Sigabrt::Types::Vector<U>& v2) {
            if (v1.size() == v2.size()){
                double dot {static_cast<double>(v1*v2)};
                double result {static_cast<double>(dot) / std::sqrt(v1.mod()*v2.mod())};
                return Sigabrt::Types::Result<double, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::OK,
                    result,
                    std::nullopt,
                    std::nullopt
                };
            }
            return Sigabrt::Types::Result<double, Sigabrt::Types::ErrorCode> {
                Sigabrt::Types::OperationType::ERR,
                std::nullopt,
                Sigabrt::Types::ErrorCode::INCOMPATIBLE_VECTORS,
                std::string("Cannot compute angle between 2 vectors of unequal dimensions.")
            };
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
         * */
        template <typename T, typename U> 
        Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> isNormalToPlane(
            const Sigabrt::Types::Plane<T>& plane,
            const Sigabrt::Types::Vector<U>& vec
        ) {
                if (vec.size() == 3){
                    const auto& normal = plane.getNormal();
                    double dot {static_cast<double>(normal*vec)};
                    double angle {static_cast<double>(dot) / std::sqrt(normal.mod()*vec.mod())};
                    bool result {angle == 1.0};
                    return Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> {
                        Sigabrt::Types::OperationType::OK,
                        result,
                        std::nullopt,
                        std::nullopt
                    };
                }
                return Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> {
                    Sigabrt::Types::OperationType::ERR,
                    std::nullopt,
                    Sigabrt::Types::ErrorCode::INCOMPATIBLE_VECTORS,
                    std::string("Only 3 dimenstional vectors can be checked for normalcy with a plane.")
                };
            }
        
    }
}

#endif
