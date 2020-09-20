#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>
#include <map>
#include <random>

#include <catch2/catch.hpp>
#include <types/fraction.hpp>

using Sigabrt::Types::Fraction;

SCENARIO("Fractions construction and conversion tests.") {
    
    GIVEN("I have 2 valid integers p and q") {
        long p {18};
        long q {24L};
        
        WHEN("I construct a fraction p/q.") {
            Fraction f {p, q};
            
            THEN("I should get a valid fraction in it's reduced form.") {
                REQUIRE(3 == f.num);
                REQUIRE(4 == f.den);
            }
            
        }
        
        WHEN("I construct a fraction p/q.") {
            
            Fraction f {p, q};
            
            AND_WHEN("I convert it to double.") {
                
                double d {f};
                
                THEN("I should be able to do so.") {
                    
                    REQUIRE(0.75 == d);
                }
            }
        }
    }
    
    GIVEN("I have 2 integers p and q.") {
        long p {24L};
        long q {0L};
        
        WHEN("I construct a fraction p/q, when q is 0.") {
            
            THEN("I should get an exception.") {
                REQUIRE_THROWS_AS(Fraction(p, q), std::invalid_argument);
            }
            
        }
    }
}

SCENARIO("Fraction additions.") {
    
    GIVEN("I have a fraction.") {
        
        Fraction f {11, 5};
        
        WHEN("I add it with integers.") {
            
            Fraction sum = {1 + f + 11};
            
            THEN("The result should be the expected fraction.") {
                REQUIRE(71 == sum.num);
                REQUIRE(5 == sum.den);
            }
        }
        
        WHEN ("I add it with other fractions.") {
            
            Fraction f1 {4, 5};
            Fraction sum {1 + f1 + f};
            
            THEN("The result should be the expected fraction.") {
                
                REQUIRE(4 == sum.num);
                REQUIRE(1 == sum.den);
            }
        }
        
    }
}

SCENARIO("Fraction negations.") {
    
    GIVEN("I have a fraction.") {
        
        Fraction f {11, 5};
        
        WHEN("I negate it.") {
            
            Fraction f1 {-f};
            
            THEN("T should get the expected fraction.") {
                
                REQUIRE(-11 == f1.num);
                REQUIRE(5 == f1.den);
            }
        }
    }
}

SCENARIO("Fraction subtractions.") {
    
    GIVEN("I have a fraction.") {
        
        Fraction f {11, 5};
        
        WHEN("I subtract an integer from it.") {
            
            Fraction sum = {10 - f - 1};
            
            THEN("The result should be the expected fraction.") {
                REQUIRE(34 == sum.num);
                REQUIRE(5 == sum.den);
            }
        }
        
        WHEN ("I add it subtract it from other fractions.") {
            
            Fraction f1 {1, 5};
            Fraction sum {10 - f - f1};
            
            THEN("The result should be the expected fraction.") {
                
                REQUIRE(38 == sum.num);
                REQUIRE(5 == sum.den);
            }
        }
        
    }
}

SCENARIO("Fraction multiplications.") {
    
    GIVEN("I have a fraction.") {
        
        Fraction f {11, 5};
        
        WHEN("I multiply it with another integer.") {
            
            Fraction sum = {3 * f *2};
            
            THEN("The result should be the expected fraction.") {
                REQUIRE(66 == sum.num);
                REQUIRE(5 == sum.den);
            }
        }
        
        WHEN ("I multiply it with other fractions.") {
            
            Fraction f1 {2, 5};
            Fraction sum {3 * f * f1};
            
            THEN("The result should be the expected fraction.") {
                
                REQUIRE(66 == sum.num);
                REQUIRE(25 == sum.den);
            }
        }
    }
}

SCENARIO("Fraction DIVISIONS.") {
    
    GIVEN("I have a fraction.") {
        
        Fraction f {11, 5};
        
        WHEN("I divide it by another integer.") {
            
            Fraction sum = {(22 / f) / 2};
            
            THEN("The result should be the expected fraction.") {
                REQUIRE(5 == sum.num);
                REQUIRE(1 == sum.den);
            }
        }
        
        WHEN ("I divide it by other fractions.") {
            
            Fraction f1 {11, 5};
            Fraction sum {3 * (f / f1)};
            
            THEN("The result should be the expected fraction.") {
                
                REQUIRE(3 == sum.num);
                REQUIRE(1 == sum.den);
            }
        }
    }
}

SCENARIO("Comparison operators.") {
    
    GIVEN("I have 3 fractions.") {
        
        Fraction f1 {1,4};
        Fraction f2 {2,4};
        Fraction f3 {3,4};
        
        WHEN("I make comparison between fractions.") {
            
            Fraction f1Eq {2, 8};
            
            THEN("I should get the expected comparisons.") {
                
                REQUIRE(f1 < f2);
                REQUIRE(f1 <= f2);
                REQUIRE(f1 != f2);
                REQUIRE(f3 > f2);
                REQUIRE(f3 >= f2);
                REQUIRE(f3 != f2);
                REQUIRE(f1Eq == f1);
            }
        }
        
        WHEN("I make comparisons between fractions and integer types.") {
            
            long i {0L};
            int j {1};
            
            THEN("I should get the expected results.") {
                REQUIRE(i < f1);
                REQUIRE(i <= f1);
                REQUIRE(i != f1);
                REQUIRE(j > f3);
                REQUIRE(j >= f2);
                REQUIRE(j != f3);
            }
        }
        
        WHEN("I make comparisons between fractions and floating point types.") {
            
            float i {0.0f};
            double j {1.0};
            
            THEN("I should get the expected results.") {
                REQUIRE(i < f1);
                REQUIRE(i <= f1);
                REQUIRE(i != f1);
                REQUIRE(j > f3);
                REQUIRE(j >= f2);
                REQUIRE(j != f3);
            }
        }
    }
}
