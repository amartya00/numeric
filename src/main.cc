#include <numeric/types/models.hpp>
#include <numeric/types/vector.hpp>
#include <numeric/math/rref.hpp>
#include <numeric/types/fraction.hpp>
#include <numeric/types/plane.hpp>
#include <numeric/types/matrix.hpp>
#include <numeric/benchmark/benchmark.hpp>
#include <numeric/math/vectorspaces.hpp>
#include <numeric/math/errors.hpp>

#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>

using Sigabrt::Types::Fraction;
using Sigabrt::Types::Vector;
using Sigabrt::Types::Plane;
using Sigabrt::Types::IsScalarType;
using Sigabrt::Types::Matrix;
using Sigabrt::Types::Result;
using Sigabrt::Numeric::ErrorCode;
using Sigabrt::Types::OperationType;
using Sigabrt::Types::Unit;
using Sigabrt::Numeric::isNormalToPlane;
using Sigabrt::Numeric::cross;
using Sigabrt::Numeric::rref;

void display(const Matrix<double>& m) {
    for (const auto& row: m) {
        for (const auto& elem: row) {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    Matrix<double> matrix {{
        {1,2},
        {7,14},
        {11,22}
    }};
    Result<Unit, ErrorCode> res {rref(matrix)};
    display(matrix);
    auto resType {res.type};
    if (resType == OperationType::OK) {
        std::cout << "OK\n";
    } else {
        std::cout << "Not OK\n";
    }
}
