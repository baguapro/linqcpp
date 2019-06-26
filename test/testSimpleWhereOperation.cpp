#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, simpleWhereFilter)
{

    auto result = processLinq(
                        from{std::move(test_data_)},
                        where{[](const person &p) { return p.age_ < 30; }}
                    );

    // confirm correct size after filter
    EXPECT_EQ(9, result.size());

    // confirm order maintained
    EXPECT_EQ("Daenerys", result.front().first_name_);
    EXPECT_EQ("Joffrey", result[1].first_name_);
    EXPECT_EQ("Ser",result.back().first_name_);

    // confirm filtering correct
    EXPECT_TRUE(existsInResult(result,[](const person &p) { return p.first_name_ == "Meera"; }));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.first_name_ == "John";}));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.first_name_ == "Ned";}));
}

TEST_F(LinqTest, twoFeatureWhereFilter)
{
    auto result = processLinq(
                        from{std::move(test_data_)},
                        where{[](const person &p) { return p.age_ < 30 && p.salary_ > 25000.00; }}
                    );

    // confirm correct size
    EXPECT_EQ(5, result.size());

    // confirm order maintained
    EXPECT_EQ("Daenerys", result.front().first_name_);
    EXPECT_EQ("Joffrey", result[1].first_name_);
    EXPECT_EQ("Grey",result.back().first_name_);

    // confirm filtering correct
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.first_name_ == "John"; }));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.first_name_ == "Ned"; }));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.first_name_ == "Ser"; }));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.first_name_ == "Meera"; }));
}

