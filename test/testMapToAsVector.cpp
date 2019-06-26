#include "linqcppTestFixture.h"

#include <gtest/gtest.h>

#include <linqcpp.h>

using namespace linqcpp_test_fixture;
using namespace linqcpp;

TEST_F(LinqTest, testMapToVector)
{
    std::map<std::string, int> data{{"test",1},{"test2",2},{"test3",3}};

    auto result = processLinq(
                        extract{[](const std::pair<std::string, int> &d)
                                { return d.first; }},
                        from{data},
                        asVector{}
                    );

    // confirm correct size
    EXPECT_EQ(3,result.size());

    // confirm correct data
    EXPECT_EQ("test",*(result.begin()));
    EXPECT_EQ("test2",result[1]);
    EXPECT_EQ("test3",result[2]);
}
