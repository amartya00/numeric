#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>
#include <map>
#include <random>
#include <tuple>

#include <catch2/catch.hpp>
#include <numeric/types/vector.hpp>
#include <numeric/types/plane.hpp>
#include <numeric/math/vectorspaces.hpp>

using Sigabrt::Types::Plane;
using Sigabrt::Types::Vector;
using Sigabrt::Numeric::cosineAngle;

SCENARIO("Plane creation from coefficients.") {
    
    GIVEN("I have 4 valid coefficients.") {
        
        double a {3.0};
        double b {5.0};
        double c {9.0};
        double k {-26.0};
        
        WHEN("I attempt to construct a plane of the form ax + by + cz = k.") {
            
            Plane<double> p1 {a,b,c,k};
            
            THEN("I should get a plane with correct normal vector, and point, and coefficients.") {
                
                Vector<double> expectedNorm {{3,5,9}};
                
                const Vector<double>& actualNorm {p1.getNormal()};
                const Vector<double>& point {p1.getPoint()};
                
                REQUIRE(1.0 == *cosineAngle(expectedNorm, actualNorm).val);
                REQUIRE(k == a*point[0] + b*point[1] + c*point[2]);
                REQUIRE(std::make_tuple(a,b,c,k) == p1.getCoefficients());
                
            }
        }
        
        WHEN("I attempt to consruct a plane with all the coefficients except k set to 0.") {
            
            THEN("I should get an exception.") {
                
                REQUIRE_THROWS_AS(Plane<double>(0.0, 0.0, 0.0, 1.0), std::invalid_argument);
            }
        }
    }
}

SCENARIO("Plane creation from normal and point.") {
    
    GIVEN("I have 2 vectors N and X0.") {
        
        Vector<double> N {{-4,-3,9}};
        Vector<double> X0 {{-5,3,-3}};
        
        WHEN("I attempt to construct a plane of the form N.(X-X0) = 0.") {
        
            Plane<double> p1 {N, X0};
            
            THEN("I should get a plane with correct normal vector, and point, and coefficients.") {
                
                Vector<double> expectedNorm {{-4,-3,9}};
                
                const Vector<double>& actualNorm {p1.getNormal()};
                const Vector<double>& point {p1.getPoint()};
                auto [a,b,c,k] = p1.getCoefficients();
                
                REQUIRE(1.0 == *cosineAngle(expectedNorm, actualNorm).val);
                
                REQUIRE(k == a*point[0] + b*point[1] + c*point[2]);
            }
        }
        
        WHEN("I attempt to construct the plane nrom non 3D vectors.") {
            
            Vector<double> N {{1,2,3,4}};
            Vector<double> X0 {{1,2,3,4}};
            
            THEN("I should get an exception.") {
                
                REQUIRE_THROWS_AS(Plane<double>(N, X0), std::invalid_argument);
            }
        }
    }
}
