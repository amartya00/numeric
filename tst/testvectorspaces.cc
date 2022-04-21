#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <functional>
#include <iostream>
#include <optional>

#include <catch2/catch.hpp>
#include <numeric/types/vector.hpp>
#include <numeric/types/models.hpp>
#include <numeric/types/plane.hpp>
#include <numeric/types/fraction.hpp>
#include <numeric/math/vectorspaces.hpp>
#include <numeric/math/errors.hpp>

using numeric::types::Fraction;
using numeric::types::Vector;
using numeric::types::Plane;
using thesoup::types::Result;
using numeric::ErrorCode;

using numeric::functions::are_linearly_dependent;
using numeric::functions::cosine_angle;
using numeric::functions::is_normal_to_plane;
using numeric::functions::cross;
using numeric::functions::linear_independence_of_system;

SCENARIO("Testing linear dependence of vectors.") {
    
    GIVEN("I have 3 vectors.") {
        
        Vector<double> v1 {{1.0, 2.0}};
        Vector<int> v2 {{-1, -2}};
        Vector<long> v3 {{1L, 55L}};
        
        WHEN("I test v1 and v2 for linear dependence.") {
            
            THEN("They should evaluate to dependent.") {
                
                Result<bool, ErrorCode> res {are_linearly_dependent(v1, v2)};
                REQUIRE(res);
                REQUIRE(res.unwrap());
            }
        }
        
        WHEN("I test v1 and v3 for linear dependence.") {
            
            THEN("They they should evaluate to linearly independent.") {
                
                Result<bool, ErrorCode> res {are_linearly_dependent(v1, v3)};
                REQUIRE(res);
                REQUIRE(!res.unwrap());
            }
        }
        
        WHEN("I test 2 vectors of unequal dimensions for linear dependence.") {
            
            Vector<int> v1 {{1,2,3}};
            Vector<int> v2 {{1,2}};
            Result<bool, ErrorCode> res {are_linearly_dependent(v1, v2)};
            
            THEN("The result should be an error.") {
                REQUIRE(!res);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == res.error());
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
                
                Result<double, ErrorCode> res {cosine_angle(v1, v2)};
                REQUIRE(res);
                REQUIRE(-1.0 == res.unwrap());
            }
        }
        
        WHEN("I test v1 and v3 for angle.") {
            
            THEN("They they should evaluate to perpendicular.") {
                
                Result<double, ErrorCode> res {cosine_angle(v1, v3)};
                REQUIRE(res);
                REQUIRE(0.0 == res.unwrap());
            }
        }
        
        WHEN("I test 2 vectors of unequal dimensions for angle.") {
            
            Vector<int> v1 {{1,2,3}};
            Vector<int> v2 {{1,2}};
            Result<double, ErrorCode> res {cosine_angle(v1, v2)};
            
            THEN("The result should be an error.") {
                REQUIRE(!res);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == res.error());
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
            
            auto res1 {is_normal_to_plane(p1, testV1)};
            auto res2 {is_normal_to_plane(p1, testV1)};
            
            THEN("The results should be as expected.") {
                REQUIRE(res1);
                REQUIRE(res2);
                REQUIRE(res1.unwrap());
                REQUIRE(res2.unwrap());
            }
        }
        
        WHEN("I try to test a vector of non-3 dimension with the plane.") {
            
            Vector<double> badVec {{1,2,3,4}};
            auto res1 {is_normal_to_plane(p1, badVec)};
            
            THEN("I should get an error.") {
                
                REQUIRE(!res1);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == res1.error());
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

                REQUIRE(3 == (result.unwrap()).size());
                REQUIRE(-42.0 == result.unwrap()[0]);
                REQUIRE(26.0 == result.unwrap()[1]);
                REQUIRE(-14.0 == result.unwrap()[2]);
            }
        }
    }
}

SCENARIO("Test linear independence of a set of vectors.") {
    
    GIVEN("I have lots of 3D vectors.") {
        
        Vector<double> v1 {{1,2,3}};
        Vector<double> v2 {{1,3,5}};
        Vector<double> v3 {{3,-1,3}};
        Vector<double> v4 {{2,4,6}};
        
        WHEN("I test linear independence of 4 of them.") {
            
            std::vector<std::reference_wrapper<Vector<double>>> input {v1, v2, v3, v4};
            Result<bool, ErrorCode> res {linear_independence_of_system(input)};
            
            THEN("The result should be false.") {
                
                REQUIRE(res);
                REQUIRE(!res.unwrap());
            }
            
        }
        
        WHEN("I test linear independence of 3 independent ones.") {
            
            std::vector<std::reference_wrapper<Vector<double>>> input {v1, v2, v3};
            Result<bool, ErrorCode> res {linear_independence_of_system(input)};
            
            THEN("The result should be true.") {
                
                REQUIRE(res);
                REQUIRE(res.unwrap());
            }
        }
        
        
        WHEN("I test linear independence of 3 non-independent ones.") {
            
            std::vector<std::reference_wrapper<Vector<double>>> input {v1, v2, v4};
            Result<bool, ErrorCode> res {linear_independence_of_system(input)};
            
            THEN("The result should be false.") {
                
                REQUIRE(res);
                REQUIRE_FALSE(res.unwrap());
            }
        }
        
        WHEN("I test linear independence of 2 independent ones.") {
            
            std::vector<std::reference_wrapper<Vector<double>>> input {v1, v2};
            Result<bool, ErrorCode> res {linear_independence_of_system(input)};
            
            THEN("The result should be true.") {
                
                REQUIRE(res);
                REQUIRE(res.unwrap());
            }
        }
        
        WHEN("I test linear independence of 2 non-independent ones.") {
            
            std::vector<std::reference_wrapper<Vector<double>>> input {v1, v4};
            Result<bool, ErrorCode> res {linear_independence_of_system(input)};
            
            THEN("The result should be false.") {
                
                REQUIRE(res);
                REQUIRE_FALSE(res.unwrap());
            }
        }
        
        WHEN("I test linear independence of 2 just one.") {
            
            std::vector<std::reference_wrapper<Vector<double>>> input {v1};
            Result<bool, ErrorCode> res {linear_independence_of_system(input)};
            
            THEN("The result should be an error.") {
                
                REQUIRE_FALSE(res);
                REQUIRE(ErrorCode::UNDERDETERMINED_SYSTEM == res.error());
            }
        }
        
        WHEN("I test linear independence of vectors on unequal dimensions.") {
            
            Vector<double> incorrect {{1,2,3,4}};
            std::vector<std::reference_wrapper<Vector<double>>> input {v1, incorrect};
            Result<bool, ErrorCode> res {linear_independence_of_system(input)};
            
            THEN("The result should be an error.") {
                
                REQUIRE_FALSE(res);
                REQUIRE(ErrorCode::INCOMPATIBLE_VECTORS == res.error());
            }
        }
    }
}
