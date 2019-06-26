#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <utility>


using namespace linqcpp_test_fixture;
using namespace linqcpp;


TEST_F(LinqTest, extractAndOrderByNoPredicate)
{
    auto result = processLinq(
                    linqcpp::extract{[](const person &p) { return make_tuple(p.last_name_, p.age_, p.salary_); }},
                    from{test_data_},
                    orderBy{}
                );

    // confirm correct size
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm correct order
    const auto &[lname0, age0, salary0] = result.front();
    EXPECT_EQ("Stark", lname0);
    EXPECT_EQ(58, age0);
    EXPECT_DOUBLE_EQ(65080.50, salary0);

    const auto &[lname1, age1, salary1] = result[1];
    EXPECT_EQ("Frey", lname1);
    EXPECT_EQ(72, age1);
    EXPECT_DOUBLE_EQ(35590.00, salary1);

    const auto &[lname2, age2, salary2] = result[2];
    EXPECT_EQ("Seaworth", lname2);
    EXPECT_EQ(61, age2);
    EXPECT_DOUBLE_EQ(32650.00, salary2);

    const auto &[lnamel, agel, salaryl] = result.back();
    EXPECT_EQ("Pounce", lnamel);
    EXPECT_EQ(5, agel);
    EXPECT_DOUBLE_EQ(1010.00, salaryl);
}

TEST_F(LinqTest, extractAndOrderByWithPredicate)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.last_name_, p.first_name_, p.salary_); }},
                    from{test_data_},
                    orderBy{[](const person &plhs, const person &prhs) { return plhs.age_ > prhs.age_; }}
                );

    // confirm correct size
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm correct order
    const auto &[lname0, fname0, salary0] = result.front();
    EXPECT_EQ("Frey", lname0);
    EXPECT_EQ("Walder", fname0);
    EXPECT_DOUBLE_EQ(35590.00, salary0);

    const auto &[lname1, fname1, salary1] = result[1];
    EXPECT_EQ("Seaworth", lname1);
    EXPECT_EQ("Davos", fname1);
    EXPECT_DOUBLE_EQ(32650.00, salary1);

    const auto &[lname2, fname2, salary2] = result[2];
    EXPECT_EQ("Stark", lname2);
    EXPECT_EQ("Ned", fname2);
    EXPECT_DOUBLE_EQ(65080.50, salary2);

    const auto &[lnamel, fnamel, salaryl] = result.back();
    EXPECT_EQ("Pounce", lnamel);
    EXPECT_EQ("Ser", fnamel);
    EXPECT_DOUBLE_EQ(1010.00, salaryl);
}

TEST_F(LinqTest, extractWithWhereAndOrderBy)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.salary_, p.first_name_, p.age_); }},
                    from{test_data_},
                    where{[](const person &p) { return p.first_name_ > "N"; }},
                    orderBy{[](const person &plhs, const person &prhs) { return plhs.salary_ > prhs.salary_; }}
                );

    // confirm correct size
    EXPECT_EQ(10, result.size());

    // confirm correct order
    const auto &[salary0, fname0, age0] = result.front();
    EXPECT_DOUBLE_EQ(65080.5,salary0);
    EXPECT_EQ("Ned", fname0);
    EXPECT_EQ(58, age0);

    const auto &[salary1, fname1, age1] = result[1];
    EXPECT_DOUBLE_EQ(55000.0, salary1);
    EXPECT_EQ("Tyrion", fname1);
    EXPECT_EQ(38, age1);

    const auto &[salary2, fname2, age2] = result[2];
    EXPECT_DOUBLE_EQ(48380.0, salary2);
    EXPECT_EQ("Petyr", fname2);
    EXPECT_EQ(56, age2);

    const auto &[salaryl, fnamel, agel] = result.back();
    EXPECT_DOUBLE_EQ(1010.0, salaryl);
    EXPECT_EQ("Ser", fnamel);
    EXPECT_EQ(5, agel);

    // confirm filtering correct
    using ResultType = std::tuple<double, std::string, int>;
    EXPECT_TRUE(existsInResult(result,[](const ResultType &rd) { return std::get<std::string>(rd) == "Podrick"; }));
    EXPECT_FALSE(existsInResult(result,[](const ResultType &rd) { return std::get<std::string>(rd) == "Meera"; }));
    EXPECT_TRUE(existsInResult(result,[](const ResultType &rd) { return std::get<std::string>(rd) == "Sandor"; }));
    EXPECT_TRUE(existsInResult(result,[](const ResultType &rd) { return std::get<std::string>(rd) == "Samwell"; }));

}
