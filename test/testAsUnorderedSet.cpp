#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, extractAndAsUnorderedSet)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.last_name_; }},
                        from{test_data_},
                        asUnorderedSet{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // confirm data intact
    EXPECT_TRUE(result.find("Snow")!=result.end());
    EXPECT_TRUE(result.find("Stark")!=result.end());
    EXPECT_TRUE(result.find("Tyrell")!=result.end());
    EXPECT_TRUE(result.find("Tarly")!=result.end());
}


TEST_F(LinqTest, asUnorderedSetWithWhereFilter)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.first_name_; }},
                        from{test_data_},
                        where{[](const person &p) { return p.salary_ < 30000; }},
                        asUnorderedSet{}
                    );

    // confirm correct size
    EXPECT_EQ(8, result.size());

    // check correct map ordering
    EXPECT_TRUE(result.find("Ramsay")!=result.end());
    EXPECT_TRUE(result.find("Podrick")!=result.end());
    EXPECT_TRUE(result.find("Sandor")==result.end());
    EXPECT_TRUE(result.find("Jorah")==result.end());
}

