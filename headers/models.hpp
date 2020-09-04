#ifndef __SIGABRT_NUMERIC_MODELS__
#define __SIGABRT_NUMERIC_MODELS__

#include <chrono>
#include <optional>
#include <string>

namespace Sigabrt {
    namespace Numeric {
        /**
         * This namespace contains various utility types that will be used throughout the rest of the library.
         **/
        namespace Models {
            /**
             * The `OperationType` enum is the type used by the `Result` struct to indicate what type of result is it 
             * returning. Possible values are `ERR` and `OK`.
             * */
            enum class OperationType {
                OK, ERR
            };
            
            /**
             * The ErrorCode enum will be used by this library as the error type for the `Result<T, E>` structure.
             * This will contain error codes specific to this library.
             * */
            enum class ErrorCode {
                UNKNOWN_ERROR,
                UNDERDETERMINED_SYSTEM,
                FREE_COLUMNS_RREF
            };
            
            /**
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
             * */
            template <typename T, typename E> struct Result {
                OperationType type {OperationType::ERR};
                std::optional<T> val {std::nullopt};
                std::optional<E> error {std::nullopt};
                std::optional<std::string> message {std::nullopt};
            };

            /**
             * The `Slice<T>` type is meant to provide functionality similar to Rust's `fat pointer` types. It has 2 fields,
             * a start pointer of type `T`, and a size. It provides some convenience overloads:
             *   - Overloaded [] operator for indexing with range checking.
             *   - Overloaded `begin` and `end` methods to enable range for loop.
             * 
             * NOTE: Move and move assignment has been disabled to avoid multiple ownership mess.
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
             * This enum is meant to emulate Rust's `Unit` type, so that we can return void types with `Result`. Example
             * `Result<Unit, ErrorCode>`
             * */
            enum class Unit{unit};
        }
    }
}

#endif
