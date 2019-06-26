#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, simpleTopAndBottomOperation)
{
    auto result = processLinq(
                        from{test_data_},
                        top{5},
                        bottom{3}
                    );

    // confirm correct size
    EXPECT_EQ(3, result.size());

    // confirm order maintained
    const auto &person2 = result.front();
    EXPECT_EQ("Daenerys",person2.first_name_);

    const auto &person3 = result[1];
    EXPECT_EQ("Tyrion", person3.first_name_);

    const auto &personl = result.back();
    EXPECT_EQ("Sandor", personl.first_name_);
}

TEST_F(LinqTest, simpleBottomAndTopOperation)
{
    auto result = processLinq(
                        from{test_data_},
                        bottom{5},
                        top{3}
                    );

    // confirm correct size
    EXPECT_EQ(3, result.size());

    // confirm order maintained
    const auto &person0 = result.front();
    EXPECT_EQ("Davos", person0.first_name_);

    const auto &person1 = result[1];
    EXPECT_EQ("Grey", person1.first_name_);

    const auto &person2 = result.back();
    EXPECT_EQ("Meera", person2.first_name_);

}

TEST_F(LinqTest, extractPairColumnWithWhereFilterAndTopBottom)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                        from{test_data_},
                        where{[](const person &p) { return (p.age_ > 30 && p.salary_ > 30000.00); }},
                        top{5},
                        bottom{2}
                );

    // confirm correct size
    EXPECT_EQ(2, result.size());

    // confirm order maintained
    const auto &[lname0, salary0] = result.front();
    const auto &person_data0 = test_data_.front();
    EXPECT_EQ("Clegane", lname0);
    EXPECT_DOUBLE_EQ(42261.8, salary0);
    
    const auto &[lnamel, salaryl] = result.back();
    const auto &person_datal = test_data_.back();
    EXPECT_EQ("Baelish", lnamel);
    EXPECT_DOUBLE_EQ(48380, salaryl);

}

TEST_F(LinqTest, extractPairColumnWithWhereFilterAndBottomTop)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                        from{test_data_},
                        where{[](const person &p) { return (p.age_ > 30 && p.salary_ > 30000.00); }},
                        bottom{5},
                        top{2}
                );

    // confirm correct size
    EXPECT_EQ(2, result.size());

    // confirm order maintained
    const auto &[lname0, salary0] = result.front();
    const auto &person_data0 = test_data_.front();
    EXPECT_EQ("Baelish", lname0);
    EXPECT_DOUBLE_EQ(48380, salary0);
    
    const auto &[lnamel, salaryl] = result.back();
    const auto &person_datal = test_data_.back();
    EXPECT_EQ("Drogo", lnamel);
    EXPECT_DOUBLE_EQ(41000.3, salaryl);

}
