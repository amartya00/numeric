#ifndef __SIGABRT_NUMERIC_ERRORS__
#define __SIGABRT_NUMERIC_ERRORS__

/**
 * \namespace numeric
 * 
 * \brief The root namespace.
 * */
namespace numeric {
    /**
     * \enum ErrorCode
     *
     * The ErrorCode enum will be used by this library as the error type for the `Result<T, E>` structure.
     * This will contain error codes specific to this library.
     * */
    enum class ErrorCode {
        UNKNOWN_ERROR,
        UNDERDETERMINED_SYSTEM,
        FREE_COLUMNS_RREF,
        INFINITE_SOLUTIONS,
        NO_SOLUTIONS,
        INCOMPATIBLE_VECTORS
    };
}

#endif


