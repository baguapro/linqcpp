#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, extractAndAsSet)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.last_name_; }},
                        from{test_data_},
                        asSet{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // confirm data intact
    EXPECT_EQ(*(result.begin()),"Baelish");
    EXPECT_EQ(*(result.rbegin()),"Worm");

}

TEST_F(LinqTest, asSetWithGivenOrdering)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.first_name_; }},
                        from{test_data_},
                        orderBy{[](const std::string &plhs, const std::string &prhs) { return plhs < prhs; }},
                        asSet{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // confirm correct order
    EXPECT_EQ("Daenerys", *(result.begin()));
    EXPECT_EQ("Walder",*(result.rbegin()));
}

TEST_F(LinqTest, asSetWithOrderingAndWhereFilter)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.first_name_; }},
                        from{test_data_},
                        where{[](const person &p) { return p.salary_ < 30000; }},
                        orderBy{[](const std::string &lhs, const std::string &rhs) { return lhs > rhs; }},
                        asSet{}
                    );

    // confirm correct size
    EXPECT_EQ(8, result.size());

    // check correct map ordering
    EXPECT_EQ("Theon", *(result.begin()));
    EXPECT_EQ("Grey", *(result.rbegin()));
}

