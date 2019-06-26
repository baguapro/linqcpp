#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, testSimpleAsDeque)
{
    auto result = processLinq(
                        from{test_data_},
                        asDeque{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // check correct order maintained
    const auto &p1 = result.front();
    EXPECT_EQ("John", p1.first_name_);

    const auto &p2 = result[1];
    EXPECT_EQ("Ned", p2.first_name_);

    const auto &pl = result.back();
    EXPECT_EQ("Walder", pl.first_name_);
}

TEST_F(LinqTest, testAsDequeWithExtract)
{
    auto result = processLinq(
                        extract{[](const person &p){ return std::make_pair(p.last_name_, p.first_name_); }},
                        from{test_data_},
                        asDeque{}
                    );

    // confirm correct size
    EXPECT_EQ(20, result.size());

    // confirm correct order
    const auto &[lastn1, firstn1] = result.front();
    EXPECT_EQ("Snow", lastn1);
    EXPECT_EQ("John", firstn1);

    const auto &[lastn2, firstn2] = result[1];
    EXPECT_EQ("Stark", lastn2);
    EXPECT_EQ("Ned",firstn2);

    const auto &[lastnl, firstnl] = result.back();
    EXPECT_EQ("Frey", lastnl);
    EXPECT_EQ("Walder", firstnl);
}
