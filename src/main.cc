#include <numeric/types/models.hpp>
#include <numeric/types/vector.hpp>
#include <numeric/math/rref.hpp>
#include <numeric/types/fraction.hpp>
#include <numeric/types/plane.hpp>
#include <numeric/types/matrix.hpp>
#include <numeric/benchmark/benchmark.hpp>
#include <numeric/math/vectorspaces.hpp>

#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>

using Sigabrt::Types::Fraction;
using Sigabrt::Types::Vector;
using Sigabrt::Types::Plane;
using Sigabrt::Types::IsScalarType;
using Sigabrt::Types::Matrix;
using Sigabrt::Numeric::isNormalToPlane;
using Sigabrt::Numeric::cross;

int main() {
    Matrix<double> A {{
        {-4, -5, 6},
        {8, 3, -4}
    }};

    Vector<double> v {{-2, 1}};
    Vector<double> res {v*A};

    std::cout << "[ ";
    for (const auto& e : res) {
        std::cout << e << " ";
    }
    std::cout << " ]\n";
}
