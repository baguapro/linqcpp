#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, orderByNoPredicate)
{
    auto result = processLinq(
                        from{test_data_},
                        orderBy{}
                );

    // confirm correct size
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm correct order
    const auto &person_res0 = result.front();
    EXPECT_EQ("Ned", person_res0.first_name_);
    EXPECT_EQ(58, person_res0.age_);
    EXPECT_DOUBLE_EQ(65080.50, person_res0.salary_);

    const auto &person_res1 = result[1];
    EXPECT_EQ("Walder", person_res1.first_name_);
    EXPECT_EQ(72, person_res1.age_);
    EXPECT_DOUBLE_EQ(35590.00, person_res1.salary_);

    const auto &person_res2 = result[2];
    EXPECT_EQ("Davos", person_res2.first_name_);
    EXPECT_EQ(61, person_res2.age_);
    EXPECT_DOUBLE_EQ(32650.00, person_res2.salary_);

    const auto person_last = result.back();
    EXPECT_EQ("Ser", person_last.first_name_);
    EXPECT_EQ(5, person_last.age_);
    EXPECT_DOUBLE_EQ(1010.00, person_last.salary_);

}

TEST_F(LinqTest, orderBySingleDataPred)
{
    auto result = processLinq(
                        from{test_data_},
                        orderBy{[](const person &plhs, const person &prhs) { return plhs.first_name_ < prhs.first_name_; }}
                );

    // confirm correct size
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm correct order
    const auto &person_res0 = result.front();
    EXPECT_EQ("Daenerys", person_res0.first_name_);

    const auto &person_res1 = result[1];
    EXPECT_EQ("Davos", person_res1.first_name_);

    const auto &person_res2 = result[2];
    EXPECT_EQ("Grey", person_res2.first_name_);

    const auto &person_last = result.back();
    EXPECT_EQ("Walder", person_last.first_name_);
}

