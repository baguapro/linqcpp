#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;


TEST_F(LinqTest, extractSingleColumnWhithWhereFilter)
{
    auto result = processLinq(
                        extract{[](const person &p) { return p.last_name_; }},
                        from{test_data_},
                        where{[](const person &p){ return p.age_ < 30; }}
                );

    // confirm correct size
    EXPECT_EQ(9, result.size());

    // confirm order maintained
    EXPECT_EQ("Targaryen", result.front());
    EXPECT_EQ("Baratheon", result[1]);
    EXPECT_EQ("Pounce",result.back());

    // confirm filtering correct
    EXPECT_TRUE(existsInResult(result,[](const std::string &lname ) { return lname == "Reed"; }));
    EXPECT_FALSE(existsInResult(result,[](const std::string &lname) { return lname == "Snow";}));
    EXPECT_FALSE(existsInResult(result,[](const std::string &lname) { return lname == "Stark";}));
}

TEST_F(LinqTest, extractPairColumnWithWhereFilter)
{
    auto result = processLinq(
                        extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                        from{test_data_},
                        where{[](const person &p) { return (p.age_ > 30 && p.salary_ > 30000.00); }}
                );

    // confirm correct size
    EXPECT_EQ(9, result.size());

    // confirm order maintained
    const auto &[lname0, salary0] = result.front();
    const auto &person_data0 = test_data_.front();
    EXPECT_EQ(person_data0.last_name_, lname0);
    EXPECT_DOUBLE_EQ(person_data0.salary_, salary0);
    
    const auto &[lname1, salary1] = result[1];
    const auto &person_data1 = test_data_[1];
    EXPECT_EQ(person_data1.last_name_, lname1);
    EXPECT_DOUBLE_EQ(person_data1.salary_, salary1);

    const auto &[lnamel, salaryl] = result.back();
    const auto &person_datal = test_data_.back();
    EXPECT_EQ(person_datal.last_name_, lnamel);
    EXPECT_DOUBLE_EQ(person_datal.salary_, salaryl);

    // confirm filtering correct
    using RES_TYPE = std::pair<std::string, double>;
    EXPECT_TRUE(existsInResult(result,[](const RES_TYPE &rdata) { return rdata.first == "Snow"; }));
    EXPECT_FALSE(existsInResult(result,[](const RES_TYPE &rdata) { return rdata.first == "H'ghar";}));
    EXPECT_FALSE(existsInResult(result,[](const RES_TYPE &rdata) { return rdata.first == "Payne";}));

}

