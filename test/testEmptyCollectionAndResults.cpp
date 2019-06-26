#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, testEmptyData)
{
    std::vector<person> empty_data;

    auto result = processLinq(
                    extract{[](const person &p) { return p.last_name_; }},
                    from{empty_data}
                );

    // confirm correct size
    EXPECT_EQ(0, result.size());
}

TEST_F(LinqTest, testEmptyResultFromWhereFilter)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.last_name_; }},
                        from{test_data_},
                        where{[](const person &p){ return p.age_ > 80; }}
                );

    // confirm correct size
    EXPECT_EQ(0, result.size());
}

TEST_F(LinqTest, testEmptyResultsAsMap)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.first_name_, p.age_); }},
                        from{test_data_},
                        where{[](const person &p) { return p.salary_ > 500000; }},
                        orderBy{[](const std::string &lhs, const std::string &rhs) { return lhs > rhs; }},
                        asMap{}
                    );

    // confirm correct size
    EXPECT_EQ(0, result.size());

    // check results is still the correct map.
    result.emplace("test1",10);
    EXPECT_EQ(1, result.size());
}
