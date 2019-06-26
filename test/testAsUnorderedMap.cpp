#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, extractAndAsUnorderedMap)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                        from{test_data_},
                        asUnorderedMap{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // confirm data intact
    EXPECT_TRUE(result.find("Snow")!=result.end());
    EXPECT_DOUBLE_EQ(result.find("Snow")->second, 50000);
    EXPECT_TRUE(result.find("Stark")!=result.end());
    EXPECT_TRUE(result.find("Tyrell")!=result.end());
    EXPECT_TRUE(result.find("Tarly")!=result.end());
}


TEST_F(LinqTest, asUnorderedMapWithWhereFilter)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.first_name_, p.age_); }},
                        from{test_data_},
                        where{[](const person &p) { return p.salary_ < 30000; }},
                        asUnorderedMap{}
                    );

    // confirm correct size
    EXPECT_EQ(8, result.size());

    // check correct data
    EXPECT_TRUE(result.find("Ramsay")!=result.end());
    EXPECT_TRUE(result.find("Podrick")!=result.end());
    EXPECT_TRUE(result.find("Sandor")==result.end());
    EXPECT_TRUE(result.find("Jorah")==result.end());
}


