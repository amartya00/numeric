#ifndef __SIGABRT_NUMERIC_VECTORSPACES__
#define __SIGABRT_NUMERIC_VECTORSPACES__

#include <types/vector.hpp>
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
         * */
        template <typename T, typename U> 
        Sigabrt::Types::Result<bool, Sigabrt::Types::ErrorCode> areLinearlyDependent(const Sigabrt::Types::Vector<T>& v1, const Sigabrt::Types::Vector<U>& v2) {
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
    }
}

#endif
