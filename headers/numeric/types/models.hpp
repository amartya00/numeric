#ifndef __SIGABRT_NUMERIC_MODELS__
#define __SIGABRT_NUMERIC_MODELS__

#include <iterator>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include <numeric/types/fraction.hpp>

/**
 * \namespace Sigabrt
 * 
 * \brief The root namespace.
 * */
namespace Sigabrt {
    /**
     * \namespace Sigabrt::Types
     * 
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace Types {        
        /**
         * \enum Operation type
         * 
         * The `OperationType` enum is the type used by the `Result` struct to indicate what type of result is it 
         * returning. Possible values are `ERR` and `OK`.
         * */
        enum class OperationType {
            OK, ERR
        };
        
        /**
         * \class Result
         * \tparam T The success type.
         * \tparam E The error type.
         * 
         * \brief A class akin to Rust's `Result<T, E>`. Helps us avoid having to throw exceptions.
         * 
         * The `Result<T, E>` type is used to return the results of operations in the library. The usage of a
         * `Result` type to return computations is borrowed from Rust, and is also a neat way to avoid the 
         * overhead of exceptions. Sibce the enumeration type of c++, as of c++17 is not so powerful as rust,
         * this is a relatively simple implementation, that avoids unnecessary complexity while getting the job
         * done.
         * 
         * Usage:
         * ------
         * If you want to return a successful result, set the `type` field to `OperationType::OK`, set the return value
         * (std::optional<T>). Note, in case of just a void return type, you can leave the val to nullopt (default value)
         * 
         * If you want to indicate error, then set the `type` field to `OperationType::ERR`, set the error to some value 
         * of type `std::optional<E>`, `E` being the error typr you use. It can be some subclass of `std::exception` or
         * your own type. This library will use the enum `ErrorCode`. You can also set the `message` to convey more useful
         * information.
         * 
         * \var type Represents the result type (Either success or err).
         * 
         * \var val Optional successful return value.
         * 
         * \var error Optional error return type.
         * 
         * \var message Optional error message
         * */
        template <typename T, typename E> struct Result {
            OperationType type {OperationType::ERR};
            std::optional<T> val {std::nullopt};
            std::optional<E> error {std::nullopt};
            std::optional<std::string> message {std::nullopt};
        };

        /**
         * \class Slice
         * 
         * \tparam T The pointer type.
         * 
         * \brief A class akin to python/rust Slice type (a fat pointer).
         * 
         * The `Slice<T>` type is meant to provide functionality similar to Rust's `fat pointer` types. It has 2 fields,
         * a start pointer of type `T`, and a size. It provides some convenience overloads:
         *   - Overloaded [] operator for indexing with range checking.
         *   - Overloaded `begin` and `end` methods to enable range for loop.
         * 
         * NOTE: Move and move assignment has been disabled to avoid multiple ownership mess.
         * 
         * \var start The start address of type `T`.
         * 
         * \var size The numer of elements in the slice.
         * */
        template <typename T> struct Slice {
            T* start;
            std::size_t size;
            
            // Default constrructor
            Slice(): start {nullptr}, size {0} {}
            
            // The copy constructor and assignment operator need to be removed. Ideally, we do not want slices to be shared.
            Slice(const Slice<T>&)=delete;
            void operator=(const Slice<T>&)=delete;
            
            T& operator[](const std::size_t& idx) {
                if (idx >= size) {
                    throw std::out_of_range("Array index out of range");
                }
                return start[idx];
            }
            
            const T& operator[](const std::size_t& idx) const {
                if (idx >= size) {
                    throw std::out_of_range("Array index out of range");
                }
                return start[idx];
            }

            T* begin() {
                return start;
            }

            const T* begin() const {
                return start;
            }

            const T* end() {
                return &start[size];
            }

            const T* end() const {
                return &start[size];
            }
        };
        
        /**
         * \enum Unit
         * 
         * \brief Represents the `void` type.
         * 
         * This enum is meant to emulate Rust's `Unit` type, so that we can return void types with `Result`. Example
         * `Result<Unit, ErrorCode>`
         * */
        enum class Unit{unit};

        /**
         * \class RunInfo 
         * 
         * \brief Encapsulates the information associated witha run of a benchmark.
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
         * \class IsForwardIteratorOfType
         * 
         * \brief Forward iterator static asserter.
         * 
         * \tparam It The iterator type. Typically you would do domething like decltype(v1.begin()).
         * 
         * \tparam T The value type that you want the iterator to have.
         * 
         * Are you creating a template for which one of the parameters has to be a forward iterator of type `T`?
         * This struct helps you with that. `IsForwardIterator<decltype(v1.begin()), int>` will evaluate to true 
         * if `v1` is a vector of integers.
         * 
         * ```
         * std::vector<int> v1 {1,2,3};
         * std::cout << std::boolalpha << Sigabrt::Numeric::Models::IsForwardIterator<decltype(v1.begin()), int>::value << "\n";
         * ```
         * */
        template <typename It,typename T, typename=void> struct IsForwardIteratorOfType {
            static constexpr bool value {false};
        };
        
        //!\cond NO_DOC
        template <typename It, typename T>
        struct IsForwardIteratorOfType<
            It, 
            T,
            typename std::enable_if<
                (
                    (
                        std::is_same<typename std::iterator_traits<It>::iterator_category, std::forward_iterator_tag>::value ||
                        std::is_same<typename std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>::value ||
                        std::is_same<typename std::iterator_traits<It>::iterator_category, std::bidirectional_iterator_tag>::value
                    ) &&
                    std::is_same<typename std::iterator_traits<It>::value_type, T>::value
                )
            >::type
        >  {
            static constexpr bool value {true};
        };
        //!\endcond
        
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
                std::is_same<Sigabrt::Types::Fraction, typename std::remove_cv<T>::type>::value
            >::type
        > {
            static constexpr bool value {true};
        };
        //! \endcond
    }

}

#endif
