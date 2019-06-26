#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, simpleBottomOperation)
{
    auto result = processLinq(
                        from{test_data_},
                        bottom{5}
                    );

    // confirm correct size
    EXPECT_EQ(5, result.size());

    // confirm order maintained
    const auto &person0 = result.front();
    EXPECT_EQ("Davos", person0.first_name_);

    const auto &person1 = result[1];
    EXPECT_EQ("Grey", person1.first_name_);

    const auto &person2 = result[2];
    EXPECT_EQ("Meera", person2.first_name_);

    const auto &person3 = result[3];
    EXPECT_EQ("Ser", person3.first_name_);

    const auto &personl = result.back();
    EXPECT_EQ("Walder", personl.first_name_);
}

TEST_F(LinqTest, bottomOperationWithWhereFilter)
{
    auto result = processLinq(
                    from{test_data_},
                    where{[](const person &p) { return p.last_name_ < "S"; }},
                    bottom{6}
                );

    // confirm correct size
    EXPECT_EQ(6, result.size());

    // confirm correct ordering
    const auto &person0 = result.front();
    EXPECT_EQ("Mormont", person0.last_name_);

    const auto &person1 = result[1];
    EXPECT_EQ("H'ghar", person1.last_name_);

    const auto &personl = result.back();
    EXPECT_EQ("Frey", personl.last_name_);

    // confirm filtering correct
    EXPECT_TRUE(existsInResult(result,[](const person &p) { return p.last_name_ == "Payne"; }));
    EXPECT_TRUE(existsInResult(result,[](const person &p) { return p.last_name_ == "Reed"; }));
    EXPECT_TRUE(existsInResult(result,[](const person &p) { return p.last_name_ == "Pounce"; }));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.last_name_ == "Snow";}));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.last_name_ == "Stark";}));
    EXPECT_FALSE(existsInResult(result,[](const person &p) { return p.last_name_ == "Greyjoy";}));


}

TEST_F(LinqTest, bottomOperationExtractAndWhereFilter)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
                    from{test_data_},
                    where{[](const person &p) { return p.salary_ < 30000.0; }},
                    bottom{3}
                );

    // confirm cirrect size
    EXPECT_EQ(3, result.size());

    // confirm correct ordering
    const auto &[fname0, age0, lname0] = result.front();
    EXPECT_EQ("Grey", fname0);
    EXPECT_EQ(23, age0);
    EXPECT_EQ("Worm", lname0);

    const auto &[fname1, age1, lname1] = result[1];
    EXPECT_EQ("Meera", fname1);
    EXPECT_EQ(14, age1);
    EXPECT_EQ("Reed", lname1);

    const auto &[fname2, age2, lname2] = result[2];
    EXPECT_EQ("Ser", fname2);
    EXPECT_EQ(5, age2);
    EXPECT_EQ("Pounce", lname2);
}

TEST_F(LinqTest, bottomOperationExtractWhereOrderingFilter)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
                    from{test_data_},
                    where{[](const person &p) { return p.salary_ > 30000.0; }},
                    orderBy{},
                    bottom{5}
                );

    // confirm correct size
    EXPECT_EQ(5, result.size());

    // confirm correct ordering
    const auto &[fname0, age0, lname0] = result.front();
    EXPECT_EQ("Jorah", fname0);
    EXPECT_EQ(51, age0);
    EXPECT_EQ("Mormont", lname0);

    const auto &[fname1, age1, lname1] = result[1];
    EXPECT_EQ("Walder", fname1);
    EXPECT_EQ(72, age1);
    EXPECT_EQ("Frey", lname1);

    const auto &[fname2, age2, lname2] = result[2];
    EXPECT_EQ("Davos", fname2);
    EXPECT_EQ(61, age2);
    EXPECT_EQ("Seaworth", lname2);

    const auto &[fname3, age3, lname3] = result[3];
    EXPECT_EQ("Margaery", fname3);
    EXPECT_EQ(24, age3);
    EXPECT_EQ("Tyrell", lname3);

    const auto &[fnamel, agel, lnamel] = result.back();
    EXPECT_EQ("Joffrey", fnamel);
    EXPECT_EQ(19, agel);
    EXPECT_EQ("Baratheon", lnamel);
}

TEST_F(LinqTest, bottomOperationExtractWhereOrderingPred)
{
    auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
                    from{test_data_},
                    where{[](const person &p) { return p.salary_ > 30000.0; }},
                    orderBy{[](const person &p1, const person &p2) { return p1.last_name_ < p2.last_name_; }},
                    bottom{4}
                );

    // confirm correct size
    EXPECT_EQ(4, result.size());

    // confirm correct ordering
    const auto &[fname0, age0, lname0] = result.front();
    EXPECT_EQ("John", fname0);
    EXPECT_EQ(32, age0);
    EXPECT_EQ("Snow", lname0);

    const auto &[fname1, age1, lname1] = result[1];
    EXPECT_EQ("Ned", fname1);
    EXPECT_EQ(58, age1);
    EXPECT_EQ("Stark", lname1);

    const auto &[fname2, age2, lname2] = result[2];
    EXPECT_EQ("Daenerys", fname2);
    EXPECT_EQ(26, age2);
    EXPECT_EQ("Targaryen", lname2);

    const auto &[fnamel, agel, lnamel] = result.back();
    EXPECT_EQ("Margaery", fnamel);
    EXPECT_EQ(24, agel);
    EXPECT_EQ("Tyrell", lnamel);
}
