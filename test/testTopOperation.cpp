#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, simpleTopOperation)
{
    auto result = processLinq(
                        from{test_data_},
                        top{5}
                    );


    // confirm correct size
    EXPECT_EQ(5, result.size());

    // confirm order maintained
    const auto &person0 = result.front();
    EXPECT_EQ("John", person0.first_name_);

    const auto &person1 = result[1];
    EXPECT_EQ("Ned", person1.first_name_);

    const auto &person2 = result[2];
    EXPECT_EQ("Daenerys",person2.first_name_);

    const auto &person3 = result[3];
    EXPECT_EQ("Tyrion", person3.first_name_);

    const auto &personl = result.back();
    EXPECT_EQ("Sandor", personl.first_name_);

}

TEST_F(LinqTest, topOperationWithWhereFilter)
{
    auto result = processLinq(
                    from{test_data_},
                    where{[](const person &p) { return p.last_name_ < "S"; }},
                    top{6}
                );

    // confirm correct size
    EXPECT_EQ(6, result.size());

    // confirm correct ordering
    const auto &person0 = result.front();
    EXPECT_EQ("Lannister", person0.last_name_);

    const auto &person1 = result[1];
    EXPECT_EQ("Clegane", person1.last_name_);

    const auto &personl = result.back();
    EXPECT_EQ("Bolton", personl.last_name_);

    // confirm filtering correct
    EXPECT_TRUE(existsInResult(result,[](const person &p) { return p.last_name_ == "Baratheon"; }));
    EXPECT_TRUE(existsInResult(result,[](const person &p) { return p.last_name_ == "Baelish"; }));
    EXPECT_TRUE(existsInResult(result,[](const person &p) { return p.last_name_ == "Drogo"; }));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.last_name_ == "Snow";}));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.last_name_ == "Stark";}));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.last_name_ == "Greyjoy";}));


}

TEST_F(LinqTest, topOperationExtractAndWhereFilter)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
                    from{test_data_},
                    where{[](const person &p) { return p.salary_ < 30000.0; }},
                    top{3}
                );

    // confirm cirrect size
    EXPECT_EQ(3, result.size());

    // confirm correct ordering
    const auto &[fname0, age0, lname0] = result.front();
    EXPECT_EQ("Ramsay", fname0);
    EXPECT_EQ(27, age0);
    EXPECT_EQ("Bolton", lname0);

    const auto &[fname1, age1, lname1] = result[1];
    EXPECT_EQ("Theon", fname1);
    EXPECT_EQ(30, age1);
    EXPECT_EQ("Greyjoy", lname1);

    const auto &[fname2, age2, lname2] = result[2];
    EXPECT_EQ("Samwell", fname2);
    EXPECT_EQ(23, age2);
    EXPECT_EQ("Tarly", lname2);
}

TEST_F(LinqTest, topOperationExtractWhereOrderingFilter)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
                    from{test_data_},
                    where{[](const person &p) { return p.salary_ > 30000.0; }},
                    orderBy{},
                    top{5}
                );

    // confirm correct size
    EXPECT_EQ(5, result.size());

    // confirm correct ordering
    const auto &[fname0, age0, lname0] = result.front();
    EXPECT_EQ("Ned", fname0);
    EXPECT_EQ(58, age0);
    EXPECT_EQ("Stark", lname0);

    const auto &[fname1, age1, lname1] = result[1];
    EXPECT_EQ("Tyrion", fname1);
    EXPECT_EQ(38, age1);
    EXPECT_EQ("Lannister", lname1);

    const auto &[fname2, age2, lname2] = result[2];
    EXPECT_EQ("John", fname2);
    EXPECT_EQ(32, age2);
    EXPECT_EQ("Snow", lname2);

    const auto &[fname3, age3, lname3] = result[3];
    EXPECT_EQ("Daenerys", fname3);
    EXPECT_EQ(26, age3);
    EXPECT_EQ("Targaryen", lname3);

    const auto &[fnamel, agel, lnamel] = result.back();
    EXPECT_EQ("Petyr", fnamel);
    EXPECT_EQ(56, agel);
    EXPECT_EQ("Baelish", lnamel);
}

TEST_F(LinqTest, topOperationExtractWhereOrderingPred)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
                    from{test_data_},
                    where{[](const person &p) { return p.salary_ > 30000.0; }},
                    orderBy{[](const person &p1, const person &p2) { return p1.last_name_ < p2.last_name_; }},
                    top{4}
                );

    // confirm correct size
    EXPECT_EQ(4, result.size());

    // confirm correct ordering
    const auto &[fname0, age0, lname0] = result.front();
    EXPECT_EQ("Petyr", fname0);
    EXPECT_EQ(56, age0);
    EXPECT_EQ("Baelish", lname0);

    const auto &[fname1, age1, lname1] = result[1];
    EXPECT_EQ("Joffrey", fname1);
    EXPECT_EQ(19, age1);
    EXPECT_EQ("Baratheon", lname1);

    const auto &[fname2, age2, lname2] = result[2];
    EXPECT_EQ("Sandor", fname2);
    EXPECT_EQ(42, age2);
    EXPECT_EQ("Clegane", lname2);

    const auto &[fnamel, agel, lnamel] = result.back();
    EXPECT_EQ("Khal", fnamel);
    EXPECT_EQ(36, agel);
    EXPECT_EQ("Drogo", lnamel);
}
