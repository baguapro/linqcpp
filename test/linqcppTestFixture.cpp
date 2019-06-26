#include "linqcppTestFixture.h"

namespace linqcpp_test_fixture
{

bool operator==(const LinqTest::person &lhs, const LinqTest::person &rhs)
{
    return (lhs.first_name_ == rhs.first_name_ &&
            lhs.last_name_ == rhs.last_name_);
}

}
