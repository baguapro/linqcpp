#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, stableUniqueFilterNoPred)
{
    auto data = test_data_;
    data.insert(data.end(), test_data_.begin(), test_data_.end());

    auto result = processLinq(
                        from{std::move(data)},
                        stableUnique{}
                );

    // confirm correct size after duplicates removed
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm order kept
    EXPECT_EQ(test_data_[1].first_name_, result[1].first_name_);
    EXPECT_EQ(test_data_[2].first_name_, result[2].first_name_);
    EXPECT_EQ(test_data_[3].first_name_, result[3].first_name_);
    EXPECT_EQ(test_data_.back().first_name_, result.back().first_name_);
}

TEST_F(LinqTest, stableUniqueFilterWithPred)
{
    auto data = test_data_;
    data.insert(data.end(), test_data_.begin(), test_data_.end());

    auto result = processLinq(
                        from{std::move(data)},
                        stableUnique{[](const person &plhs, const person &prhs) 
                                        { return (plhs.age_ == prhs.age_); }}
                );

    // confirm correct size after duplicates removed
    EXPECT_EQ(test_data_.size(), result.size());

    // confirm order kept
    EXPECT_EQ(test_data_[1].first_name_, result[1].first_name_);
    EXPECT_EQ(test_data_[2].first_name_, result[2].first_name_);
    EXPECT_EQ(test_data_[3].first_name_, result[3].first_name_);
    EXPECT_EQ(test_data_.back().first_name_, result.back().first_name_);

}

TEST_F(LinqTest, stableUniqueFilterNoPredPlainType)
{
    std::vector<int> int_data{1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};

    auto result = processLinq(
                        from{std::move(int_data)},
                        stableUnique{}
                );

    // confirm correct size
    EXPECT_EQ(9, result.size());

    // confirm order intact
    int res = 1;
    for(auto value : result)
    {
        EXPECT_EQ(res, value);
        ++res;
    }
}

TEST_F(LinqTest, stableUniqueFilterUserPredPlainType)
{
    std::vector<int> int_data{1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};

    auto result = processLinq(
                        from{std::move(int_data)},
                        stableUnique{[](int lhs, int rhs) { return lhs==rhs; }}
                );

    // confirm correct size
    EXPECT_EQ(9, result.size());

    // confirm order intact
    int res = 1;
    for(auto value : result)
    {
        EXPECT_EQ(res, value);
        ++res;
    }
}

TEST_F(LinqTest, stableUniqueWithWhereFilter)
{
    auto data = test_data_;
    data.insert(data.end(), test_data_.begin(), test_data_.end());

    auto result = processLinq(
                        from{std::move(data)},
                        where{[](const person &p) { return p.last_name_ < "N"; }},
                        stableUnique{}
                );

    // confirm correct size after duplicates removed
    EXPECT_EQ(10, result.size());

    // confirm order kept
    const auto &person0 = result.front();
    EXPECT_EQ("Lannister", person0.last_name_);

    const auto &person1 = result[1];
    EXPECT_EQ("Clegane", person1.last_name_);

    const auto &person2 = result[2];
    EXPECT_EQ("Baratheon", person2.last_name_);

    const auto &personl = result.back();
    EXPECT_EQ("Frey", personl.last_name_);
}

TEST_F(LinqTest, stableUniqueBeforeWhereFilter)
{
    auto data = test_data_;
    data.insert(data.end(), test_data_.begin(), test_data_.end());

    auto result = processLinq(
                        from{std::move(data)},
                        stableUnique{},
                        where{[](const person &p) { return p.last_name_ < "N"; }}
                );

    // confirm correct size after duplicates removed
    EXPECT_EQ(10, result.size());

    // confirm order kept
    const auto &person0 = result.front();
    EXPECT_EQ("Lannister", person0.last_name_);

    const auto &person1 = result[1];
    EXPECT_EQ("Clegane", person1.last_name_);

    const auto &person2 = result[2];
    EXPECT_EQ("Baratheon", person2.last_name_);

    const auto &personl = result.back();
    EXPECT_EQ("Frey", personl.last_name_);
}

TEST_F(LinqTest, stableUniqueExtractAndWhereFilter)
{
    auto data = test_data_;
    data.insert(data.end(), test_data_.begin(), test_data_.end());

    auto result = processLinq(
                        extract{[](const person &p) { return make_pair(p.first_name_, p.age_); }},
                        from{std::move(data)},
                        where{[](const person &p) { return (p.last_name_ > "C" && p.age_ < 50); }},
                        stableUnique{}
                );

    // confirm correct size
    EXPECT_EQ(13, result.size());

    const auto &[fname0, age0] = result.front();
    EXPECT_EQ("John", fname0);
    EXPECT_EQ(32, age0);

    const auto &[fname1, age1] = result[1];
    EXPECT_EQ("Daenerys", fname1);
    EXPECT_EQ(26, age1);

    const auto &[fname2, age2] = result[2];
    EXPECT_EQ("Tyrion", fname2);
    EXPECT_EQ(38, age2);

    const auto &[fname3, age3] = result[3];
    EXPECT_EQ("Sandor", fname3);
    EXPECT_EQ(42, age3);

    const auto &[fnamel, agel] = result.back();
    EXPECT_EQ("Ser", fnamel);
    EXPECT_EQ(5, agel);

    // confirm filtering correct
    using ResultType = std::pair<std::string, int>;
    EXPECT_TRUE(existsInResult(result,[](const ResultType &rt) { return rt.first == "Khal" && rt.second == 36; }));
    EXPECT_TRUE(existsInResult(result,[](const ResultType &rt) { return rt.first == "Margaery" && rt.second == 24; }));
    EXPECT_TRUE(existsInResult(result,[](const ResultType &rt) { return rt.first == "Jagen" && rt.second == 31; }));
    EXPECT_TRUE(existsInResult(result,[](const ResultType &rt) { return rt.first == "Podrick" && rt.second == 17; }));

    EXPECT_FALSE(existsInResult(result,[](const ResultType &rt) { return rt.first == "Ned"; }));
    EXPECT_FALSE(existsInResult(result,[](const ResultType &rt) { return rt.first == "Walder"; }));
    EXPECT_FALSE(existsInResult(result,[](const ResultType &rt) { return rt.first == "Jorah"; }));
}

TEST_F(LinqTest, preSortUniqueInts)
{
    std::vector<int> int_data{1,2,2,2,3,4,4,5,6,6,6,7,7,8,8,8,8,8,9};

    auto result = processLinq(
                        from{std::move(int_data)},
                        preSortUnique{}
                );

    // confirm correct size
    EXPECT_EQ(9, result.size());

    // confirm duplicates removed
    int res = 1;
    for(auto value : result)
    {
        EXPECT_EQ(res, value);
        ++res;
    }

}
