#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, extractSingleFieldOfTypeString)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.first_name_; }},
                        from{test_data_}
                    );


    // confirm correct size
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm order maintained
    EXPECT_EQ("John", result.front());
    EXPECT_EQ("Ned", result[1]);
    EXPECT_EQ("Walder",result.back());

}

TEST_F(LinqTest, extractSingleFieldOfTypeInt)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.age_; }},
                        from{test_data_}
                  );

    // confirm correct size
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm order maintained
    EXPECT_EQ(test_data_.front().age_, result.front());
    EXPECT_EQ(test_data_.back().age_, result.back());
}

TEST_F(LinqTest, extractTupleData)
{
    auto result = processLinq(
                        extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.salary_); }},
                        from{test_data_}
                );

    // confirm correct size
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm order maintained
    const auto &[fname0, age0, salary0] = result.front();
    EXPECT_EQ(test_data_.front().first_name_, fname0);
    EXPECT_EQ(test_data_.front().age_, age0);
    EXPECT_DOUBLE_EQ(test_data_.front().salary_, salary0);

    const auto &[fname1, age1, salary1] = result[1];
    const auto &person_data = test_data_[1];
    EXPECT_EQ(person_data.first_name_, fname1);
    EXPECT_EQ(person_data.age_, age1);
    EXPECT_DOUBLE_EQ(person_data.salary_, salary1);
}

