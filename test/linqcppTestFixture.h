#ifndef __LINQCPP_TEST_FIXTURE_H__
#define __LINQCPP_TEST_FIXTURE_H__

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <algorithm>

namespace linqcpp_test_fixture
{

class LinqTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        test_data_.insert(test_data_.end(), {
                               {"John", "Snow", 32, 50000.00},
                               {"Ned", "Stark", 58, 65080.50},
                               {"Daenerys", "Targaryen", 26, 52090.90},
                               {"Tyrion", "Lannister", 38, 55000.00},
                               {"Sandor", "Clegane", 42, 42261.80},
                               {"Joffrey", "Baratheon", 19, 31092.60},
                               {"Petyr", "Baelish", 56, 48380.00},
                               {"Khal", "Drogo", 36, 41000.30},
                               {"Ramsay", "Bolton", 27, 27044.00},
                               {"Theon", "Greyjoy", 30, 21108.10},
                               {"Jorah", "Mormont", 51, 38036.00},
                               {"Margaery", "Tyrell", 24, 32000.00},
                               {"Samwell", "Tarly", 23, 18703.70},
                               {"Jagen", "H'ghar", 31, 15080.00},
                               {"Podrick", "Payne", 17, 12800.00},
                               {"Davos", "Seaworth", 61, 32650.00},
                               {"Grey", "Worm", 23, 27500.90},
                               {"Meera", "Reed", 14, 8042.32},
                               {"Ser", "Pounce", 5, 1010.00},
                               {"Walder", "Frey", 72, 35590.00} 
                         });

    }

    virtual void TearDown()
    {
    }

public:
    // test data
    struct person
    {
        std::string first_name_;
        std::string last_name_;
        unsigned int age_;
        double salary_;

        friend bool operator < (const person &lhs, const person &rhs)
        {
            return (lhs.age_ > rhs.age_ &&
                    lhs.salary_ > rhs.salary_);
        }

    };

    std::vector<person> test_data_;
};

// helper methods
template<typename DT, typename PT>
bool existsInResult(const DT &results, PT pred)
{
    auto location_itr = std::find_if(results.begin(), results.end(), pred);
    return location_itr != results.end();
}

extern bool operator==(const LinqTest::person &lhs, const LinqTest::person &rhs);

}

namespace std
{
    template<> struct hash<linqcpp_test_fixture::LinqTest::person>
    {
        typedef linqcpp_test_fixture::LinqTest::person argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& p) const noexcept
        {
            result_type const h1 ( std::hash<std::string>{}(p.first_name_) );
            result_type const h2 ( std::hash<std::string>{}(p.last_name_) );
            return h1 ^ (h2 << 1);
        }
    };
}

#endif // __LINQCPP_TEST_FIXTURE_H__
