Table of Contents
=================

   * [Table of Contents](#table-of-contents)
   * [linqcpp](#linqcpp)
      * [Introduction](#introduction)
      * [License](#license)
      * [Implementation](#implementation)
      * [Build](#build)
      * [Test.](#test)
      * [Usage](#usage)
         * [Test data](#test-data)
         * [operator processing order](#operator-processing-order)
         * [extract a single set of data](#extract-a-single-set-of-data)
         * [extract multiple data points](#extract-multiple-data-points)
         * [where clause filter](#where-clause-filter)
         * [extractest/testTopOperation.cppt with where filter](#extract-with-where-filter)
         * [extract a pair of data with a where filter](#extract-a-pair-of-data-with-a-where-filter)
         * [orderBy with no given predicate](#orderby-with-no-given-predicate)
         * [orderBy with a given lambda predicate](#orderby-with-a-given-lambda-predicate)
         * [stableUnique with no predicate](#stableunique-with-no-predicate)
         * [stableUnique with predicate](#stableunique-with-predicate)
         * [preSortUnique with predicate](#presortunique-with-predicate)
         * [top](#top)
         * [top with where filter](#top-with-where-filter)
         * [top number greater than max available](#top-number-greater-than-max-available)
         * [top with extract and where operations](#top-with-extract-and-where-operations)
         * [bottom](#bottom)
         * [bottom with where filter](#bottom-with-where-filter)
         * [bottom with extract and where operation](#bottom-with-extract-and-where-operation)
         * [combine top and bottom operation](#combine-top-and-bottom-operation)
         * [return result as deque](#return-result-as-deque)
         * [extract a subset of data and convert result to deque](#extract-a-subset-of-data-and-convert-result-to-deque)
         * [extract data and return results as std::set](#extract-data-and-return-results-as-stdset)
         * [orderBy operator with asSet](#orderby-operator-with-asset)
         * [extract data and return results as std::map](#extract-data-and-return-results-as-stdmap)
         * [orderVy operator with asMap](#ordervy-operator-with-asmap)
         * [asUnorderedSet](#asunorderedset)
         * [asUnorderedMap](#asunorderedmap)

# linqcpp

## Introduction
linqcpp is a C++17 implementation of linq, it is designed to be simple and easy to use while trying to follow the C++ mantra of "do not pay for what you don't use". Its operation is designed to follow the order and readability of sql and linq

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

### Test data
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
                                Matt{"Daenerys", "Targaryen", 26, 52090.90},
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

### operator processing order
linqcpp will always process the *extract* operation last, all operations part from
the *from* operator will be processed in order from top to bottom or left to
right, ie. a *where* operation will be processed after an orderBy or top operator if those operators are defined before the where in the process list. See examples below.

All Examples assume ```#include <linqcpp.h>``` and ```using namespace linqcpp```

### extract a single set of data
```cpp
// result will be a std::vector<std::string>
auto result = processLinq(
                    extract{[](const person &p) { return p.first_name_; }},
                    from{test_data_}
                );

```
The library will figure out at compile time the type returned by the *'extract'* so
there will be no impact to runtime performance. (see [2] for lib and manual code
discussion)

### extract multiple data points
```cpp
// use a std::tuple to extract multiple data points, the result will be
// std::vector<std::tuple<std::string, int , double>>
auto result = processLinq(
                    extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.salary_); }},
                    from{test_data_}
            );

```

### where clause filter
```cpp
// By defaut linqcpp will return the results as a vector, later examples will show how to
// return results in different containers
auto result = processLinq(
                    from{std::move(test_data_)}, // expects a copy of the data
                    where{[](const person &p) { return p.age_ < 30; }}
                );
```
linqcpp operations are mutating operations, so takes a copy of the operating data. (see [1] for copy discussion)
The *'where'* operator object expects a lambda that will called to filter the data.
The result is returned as a vector by default. In this example the results will
be a vector of 9 people:

### extract with where filter
```cpp
// result will be a std::vector<std::string> of size 9
auto result = processLinq(
                extract{[](const person &p) { return p.last_name_; }},
                from{test_data_},
                where{[](const person &p){ return p.age_ < 30; }}
        );

```
This is basically an combination of [Example 1](#example-1---where-clause-filter) and [Example 2]((#example-2---extract-a-single-subset-of-data)

### extract a pair of data with a where filter
```cpp
// result will be a std::vector<std::pair> of size 9
auto result = processLinq(
                extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                from{test_data_},
                where{[](const person &p) { return (p.age_ > 30 && p.salary_ > 30000.00); }}
                );
```

### orderBy with no given predicate
```cpp
// order by the data's own operator<
// results will be a sorted vector
auto result = processLinq(
                from{test_data_},
                orderBy{}
        );
```

### orderBy with a given lambda predicate
```cpp
// use the given predicate to sort the data
// results will be a vector sorted by the given predicate
auto result = processLinq(
                from{test_data_},
                orderBy{[](const person &plhs, const person &prhs) 
                    { return plhs.first_name_ < prhs.first_name_; }}
        );
```
The library will check at compile time if a container has its own sort method
and if a predicate is given for the *orderBy* and genrating the appropriate
method call at compile time.

### stableUnique with no predicate
```cpp
// take a copy of the test data and duplicate the contents onto the end
auto data = test_data_;
data.insert(data.end(), test_data_.begin(), test_data_.end());

// results will be a vector matching exactly the test_data
auto result = processLinq(
                 from{std::move(data)},
                 stableUnique{}
         );
```

### stableUnique with predicate
```cpp
std::vector<int> int_data{1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};

auto result = processLinq(
                from{std::move(int_data)},
                stableUnique{[](int lhs, int rhs) { return lhs==rhs; }}
        );

```
Similar to orderBy the library will generate the appropriate method call at
compile time so there is no runtime branching.

### preSortUnique with predicate

```cpp
std::vector<int> int_data{1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9};

auto result = processLinq(
                from{std::move(int_data)},
                stableUnique{[](int lhs, int rhs) { return lhs==rhs; }}
        );

```
stableUnique uses ```std::unique``` internally 

### top 

```cpp
// top 5 elements in the data
auto result = processLinq(
                from{test_data_},
                top{5}
            );

```

### top with where filter

```cpp
auto result = processLinq(
            from{test_data_},
            where{[](const person &p) { return p.last_name_ < "S"; }},
            top{6}
        );

```
linqcpp will process the operations from top to bottom (as defined here) so the where operation
will get processed first then the top 6 of the remaining data will be selected.

### top number greater than max available

```cpp
// results will contain 6 elements as there are only 6 elements that satisfy the where filter clause
auto result = processLinq(
            from{test_data_},
            where{[](const person &p) { return (p.last_name_ < "S" && p.age_ > 35); }},
            top{7}
        );

```

### top with extract and where operations

```cpp
auto result = processLinq(
            extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
            from{test_data_},
            where{[](const person &p) { return p.salary_ < 30000.0; }},
            top{3}
        );

```
Because the *extract* operation is always processed last, the extraction of the
tuple data is only performed on the remaining 3 items obtained from the *top*
operations.

### bottom 

```cpp
// only interested in the bottom 5 elements
auto result = processLinq(
                from{test_data_},
                bottom{5}
            );

```

### bottom with where filter

```cpp
// return the bottom 6 elements after data filtered with the where operation
auto result = processLinq(
            from{test_data_},
            where{[](const person &p) { return p.last_name_ < "S"; }},
            bottom{6}
        );
```

### bottom with extract and where operation

```cpp
auto result = processLinq(
            extract{[](const person &p) { return make_tuple(p.first_name_, p.age_, p.last_name_); }},
            from{test_data_},
            where{[](const person &p) { return p.salary_ < 30000.0; }},
            bottom{3}
        );

```
Because the *extract* operation is always processed last, the extraction of the
tuple data is only performed on the remaining 3 items obtained from the *bottom*
operations.

### combine top and bottom operation

```cpp
// get the bottom 3 of the top 5 test date
auto result = processLinq(
                from{test_data_},
                top{5},
                bottom{3}
            );
```
The operation can be reversed, to get the top 3 of the bottom 5

```cpp
// get the top 3 of the bottom 5
auto result = processLinq(
                from{test_data_},
                bottom{5},
                top{3}
            );

```

### return result as deque

```cpp
// This will basically create a deque from the vector test data
auto result = processLinq(
                from{test_data_},
                asDeque{}
            );

```

### extract a subset of data and convert result to deque

```cpp
// extract a subset as a pair and return the result as a deque
auto result = processLinq(
                extract{[](const person &p){ return std::make_pair(p.last_name_, p.first_name_); }},
                from{test_data_},
                asDeque{}
            );

```
As long as the data that is extracted can be stored in a std::deque the lib will
return the result as a deque without error. As will be seen later then applies
when returning results as other container types.

### extract data and return results as std::set

```cpp
// result will be a std::set of std::string's 
auto result = processLinq(
                extract{[](const person &p) { return p.last_name_; }},
                from{test_data_},
                asSet{}
            );

```
The extracted data needs to satisfy the requirements of a std::set, ie. it needs
to have an accessible operator<

### orderBy operator with asSet

```cpp
// result will be std::set where the comparison operator is the orderBy lambda
auto result = processLinq(
                extract{[](const person &p) { return p.first_name_; }},
                from{test_data_},
                orderBy{[](const std::string &plhs, const std::string &prhs) { return plhs < prhs; }},
                asSet{}
            );

```
If An orderBy operator is used as part of the processing with a asSet
the orderBy lambda will be used as the set's comparison operation.
This will return a set in the required order, note that elements added
to the result set outside of the lib will still follow the order set by
the provided orderBy comparison lambda.

### extract data and return results as std::map

```cpp
// result will be a std::map<std::string, double> type
auto result = processLinq(
                extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                from{test_data_},
                asMap{}
            );
```
The extracted data has to be a std::pair type as the data needs to be insertable
into a std::map.

### orderBy operator with asMap

```cpp
auto result = processLinq(
                extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                from{test_data_},
                orderBy{[](const std::string &plhs, const std::string &prhs) { return plhs < prhs; }},
                asMap{}
            );
```
Excatly the same as with the asSet, if an orderBy operator is provided the 
resulting map will use the orderBy lambda as the comparsion object.

### asUnorderedSet

```cpp
auto result = processLinq(
                extract{[](const person &p) { return p.last_name_; }},
                from{test_data_},
                asUnorderedSet{}
            );
```
asUnorderedSet operates in the exact same manner as asSet. Ordering makes
no sense in unordered container so orderBy operators are ignored when used
with unordered containers.

### asUnorderedMap

```cpp
auto result = processLinq(
                extract{[](const person &p) { return std::make_pair(p.last_name_, p.salary_); }},
                from{test_data_},
                asUnorderedMap{}
            );
```
Operates in exactly the same manner as asMap except that orderBy operators
are ignored as ordering makes no sense.
