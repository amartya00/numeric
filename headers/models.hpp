#ifndef __SIGABRT_NUMERIC_MODELS__
#define __SIGABRT_NUMERIC_MODELS__

#include <optional>

namespace Sigabrt {
    namespace Numeric {
        namespace Models {
            enum class OperationType {
                OK, ERR
            };
            
            enum class ErrorCode {
                INVALID_MATRIX
            };
            
            template <typename T> struct Result {
                OperationType type;
                std::optional<T> val;
                ErrorCode error;
            };

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

            T* end() {
                return &start[size];
            }

            const T* end() const {
                return &start[size];
            }
        };
        }
    }
}

#endif
