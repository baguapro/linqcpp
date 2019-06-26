#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, extractAndAsMap)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                        from{test_data_},
                        asMap{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // confirm data intact
    EXPECT_EQ(result.begin()->first,"Baelish");
    EXPECT_DOUBLE_EQ(result.begin()->second,48380);

    EXPECT_EQ(result.rbegin()->first,"Worm");
    EXPECT_DOUBLE_EQ(result.rbegin()->second,27500.9);
}

TEST_F(LinqTest, asMapWithGivenOrdering)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                        from{test_data_},
                        orderBy{[](const std::string &plhs, const std::string &prhs) { return plhs < prhs; }},
                        asMap{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // confirm correct order
    const auto &[lastname0, salary0] = *(result.begin()); 
    EXPECT_EQ("Baelish", lastname0);
    EXPECT_DOUBLE_EQ(48380, salary0);

    const auto &[lastnamel, salaryl] = *(result.rbegin());
    EXPECT_EQ("Worm",lastnamel);
    EXPECT_DOUBLE_EQ(27500.9, salaryl);
}

TEST_F(LinqTest, asMapWithOrderingAndWhereFilter)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.first_name_, p.age_); }},
                        from{test_data_},
                        where{[](const person &p) { return p.salary_ < 30000; }},
                        orderBy{[](const std::string &lhs, const std::string &rhs) { return lhs > rhs; }},
                        asMap{}
                    );

    // confirm correct size
    EXPECT_EQ(8, result.size());

    // check correct map ordering
    const auto &[firstname0, age0] = *(result.begin());
    EXPECT_EQ("Theon", firstname0);
    EXPECT_EQ(30, age0);

    const auto &[firstnamel, agel] = *(result.rbegin());
    EXPECT_EQ("Grey", firstnamel);
    EXPECT_EQ(23, agel);
}
