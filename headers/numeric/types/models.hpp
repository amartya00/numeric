#ifndef __SIGABRT_NUMERIC_MODELS__
#define __SIGABRT_NUMERIC_MODELS__

#include <iterator>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include <numeric/types/fraction.hpp>

/**
 * \namespace numeric
 * 
 * \brief The root namespace.
 * */
namespace numeric {
    /**
     * \namespace numeric::types
     * 
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace types {
        /**
         * \class RunInfo 
         * 
         * \brief Encapsulates the information associated with run of a benchmark.
         * 
         * This class will be used for benchmarking algorithms. Each instance of the struct contain information about 1 iteration
         * of the benchmark, as it iterates over input sizes.
         * 
         * \var inputSize This represents the input size for that particular iteration.
         * 
         * \var iterations This field represents the number of iterations  to average over for that input size. 
         * Ideally you would want to have lesser iterations for large inputs.
         * 
         * \var runTime This field is an output field, representing the execution time for that iteration.
         * 
         * */
        struct RunInfo {
            unsigned long inputSize {0};
            unsigned long iterations {0};
            double runTimeInMillis {-1.0};
        };
        
        bool operator==(const RunInfo& lhs, const RunInfo& rhs) {
            return lhs.inputSize == rhs.inputSize &&
                lhs.iterations == rhs.iterations &&
                lhs.runTimeInMillis == rhs.runTimeInMillis;
        }

        /**
         * \class IsScalarType
         *
         * \brief Scalar type static asserter
         *
         * \tparam T The type under test
         *
         * This struct can be used to static assert if something is of a scalar type. This is to extend the standard scalar type asserter
         * to include other scalar types in this library like fractions.
         * */
        template <typename T, typename=void> struct IsScalarType {
            static constexpr bool value {false};
        };
        
        //! \cond NO_DOC
        template <typename T>
        struct IsScalarType<
            T,
            typename std::enable_if<
                std::is_arithmetic<T>::value ||
                std::is_same<numeric::types::Fraction, typename std::remove_cv<T>::type>::value
            >::type
        > {
            static constexpr bool value {true};
        };
        //! \endcond
    }

}

#endif
