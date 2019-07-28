#ifndef __LINQCPP_H__
#define __LINQCPP_H__

#include <string_view>
#include <algorithm>
#include <vector>
#include <deque>
#include <tuple>
#include <unordered_set>
#include <exception>
#include <experimental/type_traits>

namespace linqcpp
{
    // compile time names of the linq processes
    static constexpr auto extract_name = []() { return std::string_view{"extract"}; };
    static constexpr auto from_name = []() { return std::string_view{"from"}; };
    static constexpr auto where_name = []() { return std::string_view{"where"}; };
    static constexpr auto order_by_name = []() { return std::string_view{"order_by"}; };
    static constexpr auto to_collection_name = []()  constexpr { return std::string_view{"collection"}; };
    static constexpr auto pre_sort_unique_name = []() { return std::string_view{"pre_sort_unique"}; };
    static constexpr auto stable_unique_name = []() { return std::string_view{"stable_unique"}; };
    static constexpr auto top_name = []() { return std::string_view{"top"}; };
    static constexpr auto bottom_name = []() { return std::string_view{"bottom"}; };

    // compile time value to indicate when searched for process is not found
    static constexpr auto default_indicator_name = []() { return std::string_view{"default_indicator"}; };

    // container ids
    static constexpr auto as_vector = []() { return std::string_view{"as_vector"}; };
    static constexpr auto as_deque = []() { return std::string_view{"as_deque"}; };
    static constexpr auto as_list = []() { return std::string_view{"as_list"}; };
    static constexpr auto as_set = []() { return std::string_view{"as_set"}; };
    static constexpr auto as_map = []() { return std::string_view{"as_map"}; };
    static constexpr auto as_unordered_set = []() { return std::string_view{"as_unordered_set"}; };
    static constexpr auto as_unordered_map = []() { return std::string_view{"as_unordered_map"}; };

    // compile time check for the default predicate
    template<typename T>
    using pred_type = decltype(std::declval<T>().predicate_not_exists());

    // compile time check for existing sort method
    template<typename T>
    using contains_sort = decltype(std::declval<T>().sort());

    // compile time check for container push_back
    template<typename T, typename V>
    using contains_push_back = decltype(std::declval<T>().push_back(std::declval<V>()));

    // compile time check for container insert
    template<typename T, typename V>
    using contains_insert = decltype(std::declval<T>().insert(std::declval<V>()));

    // compile time indicator that a searched for process does not exist
    struct defaultIndicator
    {
        static constexpr auto operation = default_indicator_name();

        template<typename DT, typename RT>
        auto process(DT &&data, [[maybe_unused]] const RT &result_type)
        {
            return std::move(data);
        }

        void predicate_not_exists() { }
    };


    // return the lingcpp results as a vector
    // because asVector is the default collection used when no collection result
    // is defined it is defined / declared here so that it can be used by the
    // compile time support method below
    struct asVector
    {
        static constexpr auto operation = to_collection_name();
        static constexpr auto container_type = as_vector();

        // used by the extract process to get a requested vector type to store
        // results
        template<typename ST, typename ...TArgs>
        auto results_collection([[maybe_unused]] const ST &dummy, [[maybe_unused]] const TArgs& ...args) const
        {
            return std::vector<ST>{};
        }

        // used by process list processing to convert current collection to
        // vector if no extract operation defined
        template<typename TT, typename DT>
        auto process(const TT &tuple_data_pack, DT &&data)
        {
            auto extract_op = findOperationFromTuple(extract_name, tuple_data_pack,
                                                      std::make_index_sequence<std::tuple_size<TT>{}>{});

            // if no extract operation defined return the given data unchanged
            if constexpr(extract_op.operation!=default_indicator_name())
                return std::move(data);
            else // copy data into vector and return vector
                return std::vector<typename DT::value_type>{data.begin(), data.end()};
        }
    };

    // Support functions
    // compile time find operation terminator method
    template<typename NT>
    auto findOperation([[maybe_unused]] NT name)
    {   
        // static assert if from operation not give
        if constexpr (name()==from_name())
            static_assert(name()!=from_name(),"linqcpp - missing operation 'from'");

        // default results collection is vector ie. when no as<Collection>
        // defined in linqcpp processing
        else if constexpr (name()==to_collection_name())
            return asVector{};
        else
            return defaultIndicator{};
    }

    // compile time search for given name operation in the operations list. 
    template<typename NT, typename FT, typename ...TArgs>
    auto findOperation(NT name, const FT &first, const TArgs& ...args)
    {   
        if constexpr (name()==FT::operation)
        {
            return first; // return the operation if list head is operation searched for
        }
        else
        {
            return findOperation(name, args...); // continue search with next operation in list
        }
    }

    // search for given named operation from tuple 
    template<typename NT, typename TT, size_t... Is>
    auto findOperationFromTuple(NT name, const TT &tuple_pack, std::index_sequence<Is...>)
    {
        // expand the tuple into a parameter pack and use above find operation
        // to do the work
        return findOperation(name, std::get<Is>(tuple_pack)...);
    }

    // linqcpp result as a deque
    struct asDeque
    {
        static constexpr auto operation = to_collection_name();
        static constexpr auto container_type = as_deque();

        // used by the extract process to get a requested deque type to store
        // results
        template<typename ST, typename ...TArgs>
        auto results_collection([[maybe_unused]] const ST &dummy, [[maybe_unused]] const TArgs& ...args) const
        {
            return std::deque<ST>{};
        }

        // used by process list processing to convert current collection to
        // vector if no extract operation defined
        template<typename TT, typename DT>
        auto process(const TT &tuple_data_pack, DT &&data)
        {
            auto extract_op = findOperationFromTuple(extract_name, tuple_data_pack,
                                                      std::make_index_sequence<std::tuple_size<TT>{}>{});

            // if there is an extract operation then just return the data as the
            // extract operation will request the deque for the extract result
            if constexpr(extract_op.operation!=default_indicator_name())
                return std::move(data);
            else // copy data into and return a deque
                return std::deque<typename DT::value_type>{data.begin(), data.end()};
        }
    };

    // linqcpp result as a map
    // All the same requirements on the maps keys and values still apply
    struct asMap
    {
        static constexpr auto operation = to_collection_name();
        static constexpr auto container_type = as_map();

        // used by the extract process to get a requested map type to store
        // results.
        template<typename ST, typename ...TArgs>
        auto results_collection([[maybe_unused]] const ST &dummy, const TArgs& ...args) const
        {
            auto order_by_op = findOperation(order_by_name, args...);

            // if there is not an order by process
            if constexpr(order_by_op.operation == default_indicator_name())
                // return a std map
                return std::map<typename ST::first_type, typename ST::second_type>{};
            else
                // return a map using the order by operation as the comparison
                // operator, this will effectively be applying the order by
                // operation on the results.
                return std::map<typename ST::first_type, 
                                typename ST::second_type,
                                decltype(order_by_op.order_by_operation_)>{order_by_op.order_by_operation_};
        }

        // used by process list processing to convert current collection to
        // map if no extract operation defined
        template<typename TT, typename DT>
        auto process(const TT &tuple_data_pack, DT &&data)
        {
            auto extract_op = findOperationFromTuple(extract_name, tuple_data_pack,
                                                      std::make_index_sequence<std::tuple_size<TT>{}>{});

            if constexpr(extract_op.operation!=default_indicator_name())
                return std::move(data); // there is extract operation return data unchanged
            else
            {
                auto order_by_op = findOperationFromTuple(order_by_name, tuple_data_pack,
                                                          std::make_index_sequence<std::tuple_size<TT>{}>{});

                if constexpr(order_by_op.operation == default_indicator_name())
                    // No order by operation, copy data into map and use default
                    // operator< from key
                    return std::map<typename DT::first_type, typename DT::second_type>
                        {data.begin(), data.end()};
                else
                    // return map copying data into map using order by operation as the comparison operator
                    return std::map<typename DT::first_type, 
                                    typename DT::second_type,
                                    decltype(order_by_op.order_by_operation_)>
                                    {data.begin(), data.end()};

            }
        }
    };

    // linqcpp results as set
    struct asSet
    {
        static constexpr auto operation = to_collection_name();
        static constexpr auto container_type = as_set();

        // used by the extract process to get a requested set type to store
        // results.
        template<typename ST, typename ...TArgs>
        auto results_collection([[maybe_unused]] const ST &dummy, const TArgs ...args) const
        {
            auto order_by_op = findOperation(order_by_name, args...);

            if constexpr(order_by_op.operation == default_indicator_name())
                // no order by process, return a std set
                return std::set<ST>{};
            else
                // return a set using the order by operation as the comparison
                // operator
                return std::set<ST, 
                       decltype(order_by_op.order_by_operation_)>{order_by_op.order_by_operation_};
                
        }

        // used by process list processing to convert current collection to
        // set if no extract operation defined
        template<typename TT, typename DT>
        auto process(const TT &tuple_data_pack, DT &&data)
        {
            auto extract_op = findOperationFromTuple(extract_name, tuple_data_pack,
                                                     std::make_index_sequence<std::tuple_size<TT>{}>{});

            if constexpr(extract_op.operation!=default_indicator_name())
                // existing extract operation return data unchanged
                return std::move(data);
            else
            {
                auto order_by_op = findOperationFromTuple(order_by_name, tuple_data_pack,
                                                          std::make_index_sequence<std::tuple_size<TT>{}>{});

                if constexpr(order_by_op.operation == default_indicator_name())
                    // no order by operation, copy data into std set
                    return std::set<DT>{data.begin(), data.end()};
                else
                    // return set copying data into set using order by operation as the comparison operator
                    return std::set<DT,
                            decltype(order_by_op.order_by_operation_)>{data.begin(), data.end()};
            }
        }
    };

    // linqcpp results as unordered_map
    struct asUnorderedMap
    {
        static constexpr auto operation = to_collection_name();
        static constexpr auto container_type = as_unordered_map();

        // used by the extract process to get a requested unordered_map type to store
        // results.
        template<typename ST, typename ...TArgs>
        auto results_collection([[maybe_unused]] const ST &dummy, const TArgs ...args) const
        {
            return std::unordered_map<typename ST::first_type, typename ST::second_type>{};
        }

        // used by process list processing to convert current collection to
        // unordered_map if no extract operation defined
        template<typename TT, typename DT>
        auto process(const TT &tuple_data_pack, DT &&data)
        {
            auto extract_op = findOperationFromTuple(extract_name, tuple_data_pack,
                                                     std::make_index_sequence<std::tuple_size<TT>{}>{});

            if constexpr(extract_op.operation!=default_indicator_name())
                // existing extract operation, return data unchanged
                return std::move(data);
            else
            {
                // return unordered_map copying data in the map
                return std::unordered_map<typename DT::first_type, typename DT::second_type>
                        {data.begin(), data.end()};
            }
        }
    };

    // lingcpp results as unordered_set
    struct asUnorderedSet
    {
        static constexpr auto operation = to_collection_name();
        static constexpr auto container_type = as_unordered_set();

        // used by the extract process to get a requested unordered_set type to store
        // results.
        template<typename ST, typename ...TArgs>
        auto results_collection([[maybe_unused]] const ST &dummy, const TArgs ...args) const
        {
            return std::unordered_set<ST>{};
        }

        // used by process list processing to convert current collection to
        // unordered_set if no extract operation defined
        template<typename TT, typename DT>
        auto process(const TT &tuple_data_pack, DT &&data)
        {
            auto extract_op = findOperationFromTuple(extract_name, tuple_data_pack,
                                                    std::make_index_sequence<std::tuple_size<TT>{}>{});

            if constexpr(extract_op.operation!=default_indicator_name())
                // existing extract operation, return data unchanged
                return std::move(data);
            else
            {
                // return unordered_set copying data in the set
                return std::unordered_set<DT>{data.begin(), data.end()};
            }
        }
    };


    // extract operation
    // extracts the data defined in the predicate and puts the results into the
    // defined container or std::vector if container is not defined
    template<typename ST>
    struct extract
    {
        static constexpr auto operation = extract_name();

        ST extract_operation_;
        extract(ST &&extract_op)
            :extract_operation_(std::move(extract_op))
        { }

        // process the extraction
        // data - container to perform the extract operation on
        // args - compile time list from which to obtain the results container
        //        and the from operation to help with figuring out the type of
        //        the result.
        template<typename DT, typename ...TArgs>
        auto process(DT &&data, const TArgs& ...args)
        {
            // find if the container for the results is defined (returned
            // default is std::vector)
            auto result_type = findOperation(to_collection_name, args...);
            auto from_op = findOperation(from_name, args...);

            // obtain the results container given the type of the data to be
            // extracted
            auto results = result_type.results_collection(extract_operation_(from_op.valueTypeValue()), args...);

            if constexpr(std::experimental::is_detected<contains_push_back, 
                                                        decltype(results),
                                                        decltype(extract_operation_(from_op.valueTypeValue()))
                                                        >::value)
            {
                // results container has a push_back method so use back_inserter
                // to fill the results container
                std::transform(data.begin(), data.end(),
                            std::back_inserter(results), extract_operation_);
            }
            else if constexpr(std::experimental::is_detected<contains_insert,
                                                             decltype(results),
                                                             decltype(extract_operation_(from_op.valueTypeValue()))
                                                             >::value)
            {
                // results container has insert method use std::inserter
                std::transform(data.begin(), data.end(),
                            std::inserter(results, results.end()), extract_operation_);
            }
            else
            {
                // neither push_back or insert method exists, compile time error out
                constexpr bool error = std::experimental::is_detected<contains_push_back, 
                                                        decltype(results),
                                                        decltype(extract_operation_(from_op.valueTypeValue()))
                                                        >::value
                                       ||
                                       std::experimental::is_detected<contains_insert,
                                                             decltype(results),
                                                             decltype(extract_operation_(from_op.valueTypeValue()))
                                                             >::value;

                static_assert(error, "results container has no push_back or insert methods");
            }

            return results;
        }
    };

    // from operation
    // required process for linqcpp to work, takes a copy of the data because
    // the data can be sorted, transformed and truncated before being put into
    // another container. So design decision is to take the data by value as a
    // copy will be required anyway for any of those operations to be performed
    template<typename FT>
    struct from
    {
        static constexpr auto operation = from_name();
        using value_type = typename FT::value_type;

        FT from_data;
        from(FT from_dt)
            :from_data(std::move(from_dt))
        { }

        // The value in the data container needs to be default constructable.
        auto valueTypeValue() { return value_type{}; }

    };

    // where operation
    // to filter the data based on the where_operation predicate
    template<typename WT>
    struct where
    {
        static constexpr auto operation = where_name();

        WT where_operation_;
        where(WT &&where_op)
            :where_operation_(std::move(where_op))
        { }

        // process the where filter
        // tuple_data_pack is unused in this operation
        // data - the data on which to perform the filter operation
        template<typename TT, typename DT>
        auto process([[maybe_unused]] const TT &tuple_data_pack, DT &&data)
        {
            DT results;
            std::copy_if(data.begin(), data.end(), 
                         std::back_inserter(results), where_operation_);

            return results;
        }
    };

    // orderBy operation
    // order the data using the order_by_operation predicate or if the results
    // container is a map or set use the predicate as the comparison predicate
    template<typename OT = defaultIndicator>
    struct orderBy
    {
        static constexpr auto operation = order_by_name();

        OT order_by_operation_;
        orderBy(OT order_by_op = defaultIndicator{})
            :order_by_operation_(std::move(order_by_op))
        { }

        // process the ordering
        // compile time search the tuple_data_pack for the results container to
        // decide if the container should be sorted or if container is map or
        // set type then do not sort here as order_by_operation predicate will
        // be used by as the comparison predicate
        template<typename TT, typename DT>
        auto process(const TT &tuple_data_pack, DT &&data)
        {
            auto container_op = findOperationFromTuple(to_collection_name, tuple_data_pack,
                                                      std::make_index_sequence<std::tuple_size<TT>{}>{});

            // makes no sense to order if results container is an unordered type
            static_assert(!(container_op.container_type == as_unordered_map() ||
                            container_op.container_type == as_unordered_set()),
                            "Cannot have ordered by operations for unordered containers");

            if constexpr(container_op.container_type != as_map() &&
                         container_op.container_type != as_set() )
            {
                // if container has its own sort method ie list then use that
                if constexpr(std::experimental::is_detected<contains_sort, DT>::value)
                {
                    // if sorting predicate is not supplied, use std form of sort
                    if constexpr(std::experimental::is_detected<pred_type, OT>::value)
                        data.sort();
                    else
                        data.sort(order_by_operation_); // else use the supplied predicate
                }
                else // container doesn't have its own sorting method
                {
                    if constexpr(std::experimental::is_detected<pred_type, OT>::value)
                        std::sort(data.begin(), data.end());
                    else
                        std::sort(data.begin(), data.end(), order_by_operation_);
                }
            }

            return std::move(data);
        }
    };

    // stable unique, remove duplicates keeping order
    template<typename UT = defaultIndicator>
    struct stableUnique
    {
        static constexpr auto operation = stable_unique_name();

        UT unique_predicate_;

        stableUnique(UT unique_pred = defaultIndicator{})
            :unique_predicate_(std::move(unique_pred))
        { }

        // process the uniqueness 
        // tuple_data_pack is unused 
        // data - remove duplicates from this container
        template<typename TT, typename DT>
        auto process([[maybe_unused]] const TT &tuple_data_pack, DT &&data)
        {
            // use an unordered_set to determine if value already extsts
            auto value_set = [&](){
                                if constexpr(std::experimental::is_detected<pred_type, UT>::value)
                                {
                                    return std::unordered_set<typename DT::value_type> {};
                                }
                                else
                                {
                                    // a predicate is provided to determine
                                    // uniqueness so use that.
                                    return std::unordered_set<typename DT::value_type,
                                                              std::hash<typename DT::value_type>,
                                                              decltype(unique_predicate_)> 
                                                {1, std::hash<typename DT::value_type>{}, 
                                                    unique_predicate_};
                                }
                    }();


            auto new_end = std::remove_if(data.begin(), data.end(),
                                            [&value_set](const typename DT::value_type &value) {
                                                return !value_set.insert(value).second;
                                          });

            data.erase(new_end, data.end());

            return std::move(data);
        }

    };

    // unique operation
    // for data sets that are already sorted, quicker as there is no need to
    // keep track of duplicates
    template<typename UT = defaultIndicator>
    struct preSortUnique
    {
        static constexpr auto operation = pre_sort_unique_name();

        UT unique_predicate_;

        preSortUnique(UT unique_pred = defaultIndicator{})
            :unique_predicate_(std::move(unique_pred))
        { }

        // process the uniqueness
        // tuple_data_pack - unused in this operation
        // data - remove duplicates from this container
        template<typename TT, typename DT>
        auto process([[maybe_unused]] const TT &tuple_data_pack, DT &&data)
        {

            if constexpr(std::experimental::is_detected<pred_type, UT>::value)
            {
                auto last = std::unique(data.begin(), data.end());
                data.erase(last, data.end());
            }
            else
            {
                // use unique predicate if provided
                auto last = std::unique(data.begin(), data.end(), unique_predicate_);
                data.erase(last, data.end());
            }

            return std::move(data);
        }
    };

    // extract only the top x from given data.
    struct top
    {
        static constexpr auto operation = top_name();

        unsigned int top_number_;
        top(unsigned int number)
            :top_number_(number)
        { }

        // process top operation
        // tuple_data_pack - not used in this operation
        // data - take the top data from this container
        template<typename TT, typename DT>
        auto process([[maybe_unused]] const TT &tuple_data_pack, DT &&data)
        {
            if(data.size() > top_number_)
            {
                DT results;
                results.reserve(top_number_);

                std::copy_n(data.begin(), top_number_, std::back_inserter(results));
                return results;
            }

            return std::move(data);

        }
    };

    // extract only the bottom x from given data
    struct bottom
    {
        static constexpr auto operation = bottom_name();

        unsigned int bottom_number_;
        bottom(unsigned int number)
            :bottom_number_(number)
        { }

        // process bottom operation
        // tuple_data_pack - not used in this operation
        // data - take the bottom from this container
        template<typename TT, typename DT>
        auto process([[maybe_unused]] const TT &tuple_data_pack, DT &&data)
        {
            if(data.size() > bottom_number_)
            {
                DT results;
                results.reserve(bottom_number_);

                auto itr = data.begin();
                std::advance(itr, data.size() - bottom_number_);
                std::copy_n(itr, bottom_number_, std::back_inserter(results));

                return results;
            }

            return std::move(data);

        }
    };


    // termination method to find number of the given named operation requested.
    template<typename NT, typename OT>
    constexpr auto numberOfNamedOperations(NT op_name, const OT &linq_op)
    {
        if constexpr(OT::operation==op_name())
            return 1;
        else
            return 0;
    }

    template<typename NT, typename FT, typename ...TArgs>
    constexpr auto numberOfNamedOperations(NT op_name, const FT &first, const TArgs& ...args)
    {
        if constexpr(FT::operation == op_name())
            return numberOfNamedOperations(op_name, args...) + 1;
        else
            return numberOfNamedOperations(op_name, args...);
    }
    
    // terminator point for the std linqcpp operator processing
    template<typename TT, typename DT>
    auto processOperationSequence([[maybe_unused]] const TT &tuple_pack, DT &&data)
    {
        return std::move(data);
    }

    // clang8 is currently not correctly ignoring the other branch of the
    // constexpr if so disabling the warning for now
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wreturn-type"
    // recursively process the linqcpp operations
    template<typename TT, typename DT, typename FT, typename ...TArgs>
    auto processOperationSequence([[maybe_unused]] const TT &tuple_pack, DT &&data, FT &front, TArgs& ...args)
    {
        // if operator is not an extract or from operation (extract operation is
        // performed last and from has no operations) the perform processing and
        // continue with processing for rest in list
        if constexpr (FT::operation != extract_name() &&
                      FT::operation != from_name())
        {
            auto result = front.process(tuple_pack, std::move(data));
            return processOperationSequence(tuple_pack, std::move(result), args...);
        }
        else
        {
            return processOperationSequence(tuple_pack, std::move(data), args...);
        }

    }
    #pragma clang diagnostic pop
    
    // process 
    template<typename ...TArgs>
    auto processLinq(TArgs ...args)
    {
        static_assert(numberOfNamedOperations(to_collection_name, args...) < 2, 
                "Only one container coversion can be specified");

        static_assert(numberOfNamedOperations(order_by_name, args...) < 2,
                "Only one orderBy operation can be specified");

        static_assert(numberOfNamedOperations(extract_name, args...) < 2,
                "Only one extract operation can be specified");

        static_assert(numberOfNamedOperations(pre_sort_unique_name, args...) < 2,
                "Only one pre sorted container unique operation can be specified");

        static_assert(numberOfNamedOperations(stable_unique_name, args...) < 2,
                "Only one stable unique operation can be specified");

        auto from_op = findOperation(from_name, args...);

        // put the linqcpp operations in a tuple list so it can be used as a
        // second operations list for operation searches.
        auto tuple_pack = std::tuple<TArgs...>(args...);
        // process the linqcpp operations
        auto process_results = processOperationSequence(tuple_pack,
                                                        std::move(from_op.from_data), args...);

        auto extract_op = findOperation(extract_name, args...);
        if constexpr(decltype(extract_op)::operation!=default_indicator_name())
        {
            // extract operation performed last to ensure data contains all
            // fields to extract.
            auto extract_results = extract_op.process(std::move(process_results), args...);

            return extract_results;
        }
        else
        {
            return process_results;
        }
    }
}

#endif // __LINQCPP_H__
