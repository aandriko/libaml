/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


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


        template< template<typename...> class Less
                , typename Result
                , typename Left
                , typename Right
                >
        struct merge_;


        template< template<typename...> class Less
                , typename Result
                , typename Left
                , typename Right
                >
        struct merge_two_non_empty_lists_
        {

            struct cut_left
            {
                using type = typename merge_<   Less
                                            ,   typename Result::template rcons<typename Left::head>
                                            ,   typename Left::tail
                                            ,   Right
                                            >::type;
            };

            struct cut_right
            {
                using type = typename merge_<   Less
                                            ,   typename Result::template rcons<typename Right::head>
                                            ,   Left
                                            ,   typename Right::tail
                                            >::type;
            };



            using type = typename bool_< ! Less< typename Right::head, typename Left::head >::eval() >::
                         template conditional<    cut_left,  // left::head <= right::head
                                                  cut_right  // left::head > right::head
                                             >::type;
        };


        template< typename Result
                , typename Left
                , typename Right
                >
        struct one_side_empty_
        {
            using type  =  typename Result::
                           template apply< Left::template cons >::
                           template apply< Right::template cons >;
        };


        template< template<typename...> class Less
                , typename Result
                , typename Left
                , typename Right
                >
        struct merge_
        {
            using type  =  typename bool_< Left::size() == 0 || Right::size() == 0>::
                           template conditional<  one_side_empty_<Result, Left, Right>
                                               ,  merge_two_non_empty_lists_<Less, Result, Left, Right>
                                               >::type;
        };


        template< template<typename...> class Less >
        struct bind_less
        {
            template<typename... X>
            struct merge_sort_;


            template<typename... X>
            struct sort_more_than_one_
            {
                using left   =  typename list<X...>::
                                template take<  _<sizeof...(X)/2>  >::
                                template apply<merge_sort_>::type;

                using right  =  typename list<X...>::
                                template drop<  _<sizeof...(X)/2>  >::
                                template apply<merge_sort_>::type;

                static_assert( sizeof...(X) == left::size() + right::size() );

                using type   =  typename merge_<Less, list<>, left, right>::type;
            };


            template<typename... X>
            struct sort_one_or_less_
            {
                static_assert(sizeof...(X) <= 1);
                using type  =  list<X...>;
            };


            template< typename... X >
            struct merge_sort_
            {
                using type  =  typename bool_< (sizeof...(X) > 1) >::
                               template conditional< sort_more_than_one_<X...>, sort_one_or_less_<X...> >::type;

            };
        };

    public:

        template<  template<typename...> class  >
        using with = list<>;

    };


    template< typename... X >
    struct sort
    {
        template<  template< typename... > class Less  >
        using with  =  typename sort<>::template bind_less<Less>::template merge_sort_<X...>::type;
    };

}

#include "./list.hpp"
