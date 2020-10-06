#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>
#include <optional>

#include <catch2/catch.hpp>
#include <types/vector.hpp>
#include <types/models.hpp>
#include <types/plane.hpp>
#include <types/fraction.hpp>
#include <math/vectorspaces.hpp>

using Sigabrt::Types::Fraction;
using Sigabrt::Types::Vector;
using Sigabrt::Types::Plane;
using Sigabrt::Types::Result;
using Sigabrt::Types::ErrorCode;
using Sigabrt::Types::OperationType;

using Sigabrt::Numeric::areLinearlyDependent;
using Sigabrt::Numeric::cosineAngle;
using Sigabrt::Numeric::isNormalToPlane;
using Sigabrt::Numeric::cross;

SCENARIO("Testing linear dependence of vectors.") {
    
    GIVEN("I have 3 vectors.") {
        
        Vector<double> v1 {{1.0, 2.0}};
        Vector<int> v2 {{-1, -2}};
        Vector<long> v3 {{1L, 55L}};
        
        WHEN("I test v1 and v2 for linear dependence.") {
            
            THEN("They should evaluate to dependent.") {
                
                Result<bool, ErrorCode> res {areLinearlyDependent(v1, v2)};
                REQUIRE(OperationType::OK == res.type);
                REQUIRE(*res.val);
                REQUIRE(std::nullopt == res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
        
        WHEN("I test v1 and v3 for linear dependence.") {
            
            THEN("They they should evaluate to linearly independent.") {
                
                Result<bool, ErrorCode> res {areLinearlyDependent(v1, v3)};
                REQUIRE(OperationType::OK == res.type);
                REQUIRE(!*res.val);
                REQUIRE(std::nullopt == res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
        
        WHEN("I test 2 vectors of unequal dimensions for linear dependence.") {
            
            Vector<int> v1 {{1,2,3}};
            Vector<int> v2 {{1,2}};
            Result<bool, ErrorCode> res {areLinearlyDependent(v1, v2)};
            
            THEN("The result should be an error.") {
                REQUIRE(OperationType::ERR == res.type);
                REQUIRE(std::nullopt == res.val);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == *res.error);
                REQUIRE(
                    std::string(
                        "Cannot check linear independence of 2 vectors of unequal dimensions."
                    ).compare(*res.message) == 0
                );
            }
        }
    }
}

SCENARIO("Testing computation of angles between 2 vectors.") {
    
    GIVEN("I have 3 vectors.") {
        
        Vector<double> v1 {{1.0, 1.0}};
        Vector<int> v2 {{-2, -2}};
        Vector<long> v3 {{-1L, 1L}};
        
        WHEN("I test v1 and v2 for angle.") {
            
            THEN("They should anti-parallel.") {
                
                Result<double, ErrorCode> res {cosineAngle(v1, v2)};
                REQUIRE(OperationType::OK == res.type);
                REQUIRE(-1.0 == *res.val);
                REQUIRE(std::nullopt == res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
        
        WHEN("I test v1 and v3 for angle.") {
            
            THEN("They they should evaluate to perpendicular.") {
                
                Result<double, ErrorCode> res {cosineAngle(v1, v3)};
                REQUIRE(OperationType::OK == res.type);
                REQUIRE(!*res.val);
                REQUIRE(std::nullopt == res.error);
                REQUIRE(std::nullopt == res.message);
            }
        }
        
        WHEN("I test 2 vectors of unequal dimensions for angle.") {
            
            Vector<int> v1 {{1,2,3}};
            Vector<int> v2 {{1,2}};
            Result<double, ErrorCode> res {cosineAngle(v1, v2)};
            
            THEN("The result should be an error.") {
                REQUIRE(OperationType::ERR == res.type);
                REQUIRE(std::nullopt == res.val);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == *res.error);
                REQUIRE(
                    std::string(
                        "Cannot compute angle between 2 vectors of unequal dimensions."
                    ).compare(*res.message) == 0
                );
            }
        }
    }
}

SCENARIO("Testing vectors normal to planes.") {
    
    GIVEN("I have 2 3D vectors and a plane.") {
        
        Vector<double> testV1 {{2,4,6}};
        Vector<double> testV2 {{2,4,11}};
        Plane<double> p1 {1,2,3,7};
        
        WHEN("I test if the vectors are nirmal to the plane.") {
            
            auto res1 {isNormalToPlane(p1, testV1)};
            auto res2 {isNormalToPlane(p1, testV1)};
            
            THEN("The results should be as expected.") {
                REQUIRE(OperationType::OK == res1.type);
                REQUIRE(OperationType::OK == res2.type);
                REQUIRE(*res1.val);
                REQUIRE(*res2.val);
            }
        }
        
        WHEN("I try to test a vector of non-3 dimension with the plane.") {
            
            Vector<double> badVec {{1,2,3,4}};
            auto res1 {isNormalToPlane(p1, badVec)};
            
            THEN("I should get an error.") {
                
                REQUIRE(OperationType::ERR == res1.type);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == *res1.error);
                REQUIRE(
                    std::string(
                        "Only 3 dimenstional vectors can be checked for normalcy with a plane."
                    ).compare(*res1.message) == 0
                );
            }
        }
    }
}

SCENARIO("Testing cross products.") {

    GIVEN("I have 2 3 dimensional vectors.") {

        Vector<double> v1 {{6,7,-5}};
        Vector<Fraction> v2 {{{8,1}, {7,1}, {-11,1}}};

        WHEN("I compute the cross product.") {

            auto result {cross(v1,v2)};

            THEN("The result should be the expected vector.") {

                REQUIRE(3 == (*result.val).size());
                REQUIRE(-42.0 == (*result.val)[0]);
                REQUIRE(26.0 == (*result.val)[1]);
                REQUIRE(-14.0 == (*result.val)[2]);
            }
        }
    }
}
