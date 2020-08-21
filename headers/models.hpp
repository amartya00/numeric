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
        }
    }
}

#endif
