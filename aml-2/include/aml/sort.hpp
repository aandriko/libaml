#pragma once

namespace aml
{
    template< typename... >
    struct list;

    template<typename...>
    struct sort;


    template<>
    struct sort<>
    {
    private:

        template<typename...>
        friend struct sort;



        // quick sort for types
        template<typename List, template<typename...> class Less>
        struct sort_list_
        {
            struct sort_non_empty_list_
            {
                using pivot = typename List::head;

                template<typename X>
                using predicate = Less<X, pivot>;

                using partition = typename List::tail::template partition_with< predicate >;

                using lower = typename sort_list_<typename partition::accepted, Less>::type;
                using upper = typename sort_list_<typename partition::rejected, Less>::type;

                using type = typename upper::template apply<lower::template rcons<pivot>::template rcons>;
            };

            struct sort_empty_list_
            {
                using type = list<>;
            };

            using type = typename conditional< bool_<List::size() != 0>,
                                               sort_non_empty_list_,
                                               sort_empty_list_ >::type;
        };


        // We add stability to quicksort bysorting pairs of types and their index in the original list
        template<typename X, auto n>
        struct stabilized
        {
            static constexpr auto index() { return n; }
            using type = X;
        };


        template<typename List, typename X>
        using add_to_stabilizer  =  typename List::template rcons< stabilized<X, List::size()> >;

        template<typename... X>
        using stabilized_list = typename list<X...>::template lfold_with<add_to_stabilizer, list<> >;

        template<template<typename...> class Less>
        struct stabilized_less
        {
            template<typename Left, typename Right>
            using apply_to  =  bool_<    Less< typename Left::type, typename Right::type >::eval()
                                         ||
                                         ( ! Less< typename Right::type, typename Left::type >::eval()
                                           &&
                                           Left::index() < Right::index() )    >;
        };

        template<typename X>
        using remove_stabilizer = typename X::type;

    public:

        template< template<typename...> class >
        using with = list<>;
    };


    template<typename... X>
    struct sort
    {
        template< template<typename...> class Less >
        using with  =  typename sort<>::
                       template sort_list_< typename sort<>::template stabilized_list<X...>,
                                            sort<>::template stabilized_less<Less>::template apply_to >::
                       type::
                       template pointwise_apply< sort<>::template remove_stabilizer >;


    };
}

#include "./list.hpp"
