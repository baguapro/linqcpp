# linqcpp

## Introduction
linqcpp is a C++17 implementation of linq, it is designed to be simple and easy to use while trying to follow the C++ mantra of "do not pay for what you don't use".

## License
linqcpp is licensed under the terms of the MIT license, see LICENSE.md for details

## Implementation
The library is implemented in a single header src/linqcpp.h and uses the latest experimental C++17 features and so it is NOT recommended for production use.

## Build
linqcpp is a single header library and needs to be built with a C++17 compliant compiler for clang and gcc set the -std=c++17 flag compiler flag

## Test.
To build the test go in the test directory and run:

```
mkdir build
cd build
cmake ..
make run_tests
```
The tests require googletest and the make process will download and build the googletest library as part of the test build and run process.

## Usage
All the linqcpp code is inside the linqcpp namespace

Tests data
```cpp
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
```
### Example 1 - filter collection data with the where clause
```cpp
#include <linqcpp>

using namespace linqcpp;
// By defaut linqcpp will return the results as a vector, later examples will show how to
// return results in different containers
auto result = processLinq(
                    from{std::move(test_data_)},    // expects a copy of the
data
                    where{[](const person &p) { return p.age_ < 30; }}
                );
```
linqcpp operations are mutating operations, so takes a copy of the operating data. (see [1] for copy discussion)

The *'where'* operator object expects a lambda that will called to filter the data.

The result is returned as a vector by default. In this example the results will
be a vector of 9 people:

```
{"Daenerys", "Targaryen", 26, 52090.90},
{"Joffrey", "Baratheon", 19, 31092.60},
{"Ramsay", "Bolton", 27, 27044.00},
{"Margaery", "Tyrell", 24, 32000.00},
{"Samwell", "Tarly", 23, 18703.70},
{"Podrick", "Payne", 17, 12800.00},
{"Grey", "Worm", 23, 27500.90},
{"Meera", "Reed", 14, 8042.32},
{"Ser", "Pounce", 5, 1010.00},
```

### Example 2 - Extract a single subset of data
```cpp
#include <linqcpp.h>

using namespace linqcpp;
// result will be a vector of strings
auto result = processLinq(
                    extract{[](const person &p) { return p.first_name_; }},
                    from{test_data_}
                );

```
The library will figure out at compile time the type returned by the *'extract'* so
there will be no impact to runtime performance. (see [2] for lib and manual code
discussion)

*'result'* will be a std::vector<std::string> containing:
```
/* results are
"John"
"Ned"
"Daenerys"
"Tyrion"
"Sandor"
"Joffrey"
"Petyr"
"Khal"
"Ramsay"
"Theon"
"Jorah"
"Margaery"
"Samwell"
"Jagen"
"Podrick"
"Davos"
"Grey"
"Meera"
"Ser"
"Walder"
```

### Example 3 - Extract multiple points of data from the original operating data
```cpp
#include <linqcpp.h>

using namespace liqcpp
// use a std::tuple to extract multiple data points, the result will be
// std::vector<std::tuple<std::string, int , double>>
auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.salary_); }},
                    from{test_data_}
            );


```
*'result'* as expects is:
```
"John",  32, 50000.00
"Ned", 58, 65080.50
"Daenerys", 26, 52090.90
"Tyrion", 38, 55000.00
"Sandor", 42, 42261.80
"Joffrey", 19, 31092.60
"Petyr", 56, 48380.00
"Khal", 36, 41000.30
"Ramsay", 27, 27044.00
"Theon", 30, 21108.10
"Jorah", 51, 38036.00
"Margaery", 24, 32000.00
"Samwell", 23, 18703.70
"Jagen", 31, 15080.00
"Podrick", 17, 12800.00
"Davos", 61, 32650.00
"Grey", 23, 27500.90
"Meera", 14, 8042.32
"Ser", 5, 1010.00
"Walder", 72, 35590.00

```
