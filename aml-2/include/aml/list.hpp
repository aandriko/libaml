#pragma once

#include "./functional.hpp"

namespace aml
{
    template< typename H, typename... T>
    struct head_and_tail
    {
        using head = H;


        struct tail
        {
            using type     =  typename head_and_tail<T...>::tail;
            using return_  =  head_and_tail< H, T... >;
        };


        template<  template<typename...> class F >
        using apply  = F< H, T... >;
    };


    template<>
    struct head_and_tail
    {
        template<  template< typename... > class F  >
        using apply = F<>;
    };


    template<typename... X>
    using head = typename head_and_tail<X...>::head;


    template< auto n, typename... X  >
    using at  =  typename power< typename head_and_tail<X...>::tail, n >::return_::template apply<head>;


    template< typename N, typename... X>
    using at_l = at< N::eval(), X... >;


    template< typename... >
    struct list;


    template<>
    struct list<>
    {
        static constexpr auto size() { return 0; }

        template< typename... X >
        using cons  =  list< X... >;

        template< typename... X >
        using rcons  =  list< X... >;

        using reverse  =  list<>;


        template< template< typename... > class F >
        using apply  =  F<>;


        template< template< typename... > class >
        using pointwise_apply  =  list<>;


        template< template< typename... > class
                , typename Z
                >
        using rscan_with  =  Z;


        template< template< typename... > class
                , typename Z
                >
        using lscan_with  =  Z


        template< template< typename... > class
                , typename Z
                >
        using rfold_with = Z;


        template< template<typename...> class
                , typename Z
                >
        using lfold_with = Z;


        template< template< typename...> class
                , typename b
                >
        using map_accum_left_with  =  list<b, conslist<> >;


        template< template< tyepname...> class
                , typename b
                >
        uisng map_accum_right_with  = list<b, conslist<> >;


        template<auto n>
        using drop  =  typename bool_< n == 0 >::template enable< list<> >;


        template<auto n>
        using take  =  typename bool_< n == 0 >::template enable< list<> >;


        template<template<typename...> class>
        using split_by_first_occurence_of  =  list< list<>, list<> >;


        template<template<typename...> class>
        struct partition_with : public list< list<>, list<> >
        {
            using accepted  =  list<>;
            using rejected  =  list<>;
        };


        template<template<typename...> class >
        using sort_with  =  list<>;


        template< template<typename...> class >
        using take_while = list<>;


        template< template<typename...> class >
        using drop_while = list<>;
    };
}
