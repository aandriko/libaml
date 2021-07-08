/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "./functional.hpp"
#include "./logic.hpp"
#include "./object.hpp"
#include "./sort.hpp"


namespace aml
{
    template< typename... X >
    struct head_and_tail;

    template<>
    struct head_and_tail<>
    {
        struct tail
        {
            using type     =  tail;
            using return_  =  head_and_tail<>;
        };

        template<  template<typename...> class F  >
        using apply = F<>;
    };


    template< typename H, typename... T>
    struct head_and_tail<H, T...>
    {
        using head = H;


        struct tail
        {
            using type     =  typename head_and_tail<T...>::tail;
            using return_  =  head_and_tail< H, T... >;        // This includes the head!
        };


        template<  template<typename...> class F  >
        using apply  = F< H, T... >;
    };



    template<typename... X>
    using head = typename head_and_tail<X...>::head;


    template< typename n, typename... X  >
    using at  =  typename power< typename head_and_tail<X...>::tail, n >::return_::template apply<head>;


    template< typename    List
            , typename... no_args
            >
    using list_head  = typename identity<List, no_args...>::head;


    template< typename    List
            , typename... no_args
            >
    using list_tail  =  typename identity<List, no_args...>::tail;


    template< typename List, typename... no_args >
    using list_last  =  typename identity<List, no_args...>::last;


    template< typename List
            , typename... no_args
            >
    using list_init  =  typename identity< List, no_args... >::init;


    template< typename    List
            , typename... no_args
            >
    using list_relaxed_head  = typename identity<List, no_args...>::relaxed_head;


    template< typename    List
            , typename... no_args
            >
    using list_relaxed_tail  =  typename identity<List, no_args...>::relaxed_tail;


    template< typename List, typename... no_args >
    using list_relaxed_last  =  typename identity<List, no_args...>::relaxed_last;


    template< typename List
            , typename... no_args
            >
    using list_relaxed_init  =  typename identity< List, no_args... >::relaxed_init;


    template< typename... >
    struct list;


    template<>
    struct list<>
    {
        struct no_content;

        using relaxed_head  =  no_content;
        using relaxed_tail  =  list<>; //no_content;
        using relaxed_init  =  list<>; //no_content;
        using relaxed_last  =  no_content;


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
        using rscan_with  =  list<Z>;


        template< template< typename... > class
                , typename Z
                >
        using lscan_with  =  list<Z>;


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
        using map_accum_left_with  =  list<b, list<> >;


        template< template< typename...> class
                , typename b
                >
        using map_accum_right_with  = list<b, list<> >;


        template<typename N>
        using drop  =  typename bool_< N::eval() == 0 >::template enable< list<> >;


        template<typename N>
        using take  =  typename bool_< N::eval() == 0 >::template enable< list<> >;


        template<  template<typename...> class  >
        using drop_while  =  list<>;


        template<  template<typename...> class  >
        using take_while  =  list<>;


        template<template<typename...> class>
        struct partition_with
        {
            using accepted  =  list<>;
            using rejected  =  list<>;

            template<  template< typename... > class F  >
            using apply = F< accepted, rejected >;
        };


        template<template<typename...> class>
        struct split_by_first_occurence_of
        {
            using prefix = list<>;
            using suffix = list<>;
        };


        template< template<typename...> class >
        using sort_with = list<>;


        template<typename... X>
        list<X...> operator+(list<X...>);

    };


    template< typename    H
            , typename... T
            >
    struct list< H, T... >
    {
        static constexpr auto size()  { return 1 + sizeof...(T); }


        template< typename... X >
        using cons  =  list< X..., H, T... >;


        template< typename... X >
        using rcons  =  list< H, T..., X... >;


        using reverse =  typename list< T... >::reverse::
                         template rcons< H >;


        using head  =  H;


        using tail  =  list< T... >;


        using init  =  typename reverse::tail::reverse;


        using last  =  typename reverse::head;


        using relaxed_head = head;


        using relaxed_tail = tail;


        using relaxed_init = init;


        using relaxed_last = last;


        template<  template< typename... > class F  >
        using apply  =  F< H, T... >;


        template<  template< typename... > class F  >
        using pointwise_apply  =  list< F<H>, F<T>... >;


        template<  template< typename... > class F
                , typename Z
                >
        using rfold_with  =  F< head, typename tail::template rfold_with<F, Z> >;


        template<  template< typename... > class F
                , typename Z
                >
        using lfold_with  =  F< typename init::template lfold_with<F, Z>, last >;


        template<  template< typename... > class F
                ,  typename Z
                >
        using rscan_with  =  typename tail::template rscan_with< F, Z >::
                             template cons< F< head, typename tail::template rscan_with<F, Z>::head > >;


        template<  template< typename... > class F
                ,  typename Z
                >
        using lscan_with  =  typename init::template lscan_with< F, Z >::
                             template rcons<  F< typename init::template lscan_with<F, Z>::last, last >  >;



    private:

        // maccL: [ B x A --F--> B x C ] x B --> [A] --> B x [C]
        template<  template< typename...> class F
                ,  typename B
                ,  typename b     =  typename F<B, head>::head
                ,  typename c     =  typename F<B, head>::tail::head
                ,  typename gamma =  typename tail::template map_accum_left_with<F, b>
                >
        struct map_accum_left_with_
        {
            using type =  aml::list< typename gamma::head,
                                     typename gamma::tail::head::template cons<c> >;
        };


        // maccR: [ B x A --F--> B x C ] x B --> [A] --> B x [C]
        template<  template< typename...> class F
                ,  typename B
                ,  typename b     =  typename F<B, last>::head
                ,  typename c     =  typename F<B, last>::tail::head
                ,  typename gamma =  typename init::template map_accum_right_with<F, b>
                >
        struct map_accum_right_with_
        {
            using type =  aml::list< typename gamma::head,
                                     typename gamma::tail::head::template cons<c> >;
        };


    public:

        // maccL: [ B x A --F--> B x C ] x B --> [A] --> B x [C]
        template< template<typename...> class F
                , typename B>
        using map_accum_left_with = typename map_accum_left_with_<F, B>::type;


        // maccL: [ B x A --F--> B x C ] x B --> [A] --> B x [C]
        template<template< typename...> class F
                , typename B >
        using map_accum_right_with = typename map_accum_right_with_<F, B>::type;


        template< typename N >
        using drop  =  typename monoid<list_relaxed_tail>::template power<N>::template apply_to<list<H, T...> >;


        template< typename N >
        using take  =  typename monoid<list_relaxed_tail>::template power< num<size() - N::eval() > >::template apply_to< reverse >::reverse;


        template<typename... Y>
        list<H, T..., Y...> operator+(list<Y...>);

    private:

        template<typename Pair>
        struct add_to_first
        {
            template< typename X >
            using apply_to  =  list<  typename Pair::head::template rcons<X>,
                                      typename Pair::tail::head  >;
        };

        template< typename Pair >
        struct add_to_second
        {
            template< typename X >
            using apply_to  =  list<  typename Pair::head,
                                      typename Pair::tail::head::
                                      template rcons< X > >;
        };

        template<  template<typename... > class Pred  >
        struct fix_predicate
        {
            template< typename Pair
                    , typename X
                    >
            using add_to_pair  =  typename conditional< bool_<Pred<X>::eval() >,
                                                        add_to_first<Pair>,
                                                        add_to_second<Pair> >::
                                  template apply_to<X>;
        };

    public:

        template<  template< typename... > class Predicate  >
        struct partition_with
        {
        private:
            using partition = lfold_with< fix_predicate<Predicate>::template add_to_pair, list< list<>, list<> > >;

        public:
            using accepted  =  typename partition::head;
            using rejected  =  typename partition::tail::head;

            template<  template< typename... > class F  >
            using apply  =  F< accepted, rejected >;
        };


        template<template<typename...> class Pred>
        struct split_by_first_occurence_of
        {
        private:

            using Z = list< list<>, list<> >;

            struct add_to_prefix
            {
                template< typename Scanned, typename A >
                using action  =  list< typename Scanned::head::template rcons<A>, typename Scanned::tail::head >;
            };

            struct add_to_suffix
            {
                template< typename Scanned, typename A >
                using action  =  list< typename Scanned::head, typename Scanned::tail::head::template rcons<A> >;
            };

            template<typename Scanned, typename A>
            using add  =  typename conditional<    bool_< Pred<A>::eval() || Scanned::tail::head::size() != 0 >,
                                                   add_to_suffix,
                                                   add_to_prefix    >::template action< Scanned, A >;


            using partition  =  lfold_with< add, Z >;

        public:

            using prefix  =  typename partition::head;
            using suffix  =  typename partition::tail::head;
        };


        template< template<typename...> class Less >
        using sort_with  =  typename aml::sort<H, T...>::template with< Less >;


        template< template<typename...> class Pred>
        using take_while = typename split_by_first_occurence_of< predicates::none<Pred>::template apply_to >::prefix;


        template< template<typename...> class Pred>
        using drop_while = typename split_by_first_occurence_of< predicates::none<Pred>::template apply_to >::suffix;
    };

    template<typename List, typename... F>
    using list_apply  =  typename List::template apply< identity<F...>::template apply_to >;


    template<typename List, typename... F>
    using list_pointwise_apply  =  typename List::template pointwise_apply< identity<F...>::template apply_to >;


    template<typename... List>
    using join = decltype( ( List() + ... + list<>() ) );


    template< typename List, typename... X>
    using list_cons  =  typename List::template cons<X... >;


    template< typename List, typename... X>
    using list_rcons  =  typename List::template rcons<X... >;


    template< typename... List >
    using list_reverse  =  typename identity<List...>::reverse;


    template< typename List, typename F, typename... Z >
    using list_rscan = typename List::template rscan_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... Z >
    using list_lscan = typename List::template lscan_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... Z >
    using list_rfold  =  typename List::template rfold_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... Z >
    using list_lfold  =  typename List::template lfold_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... B>
    using list_map_accum_left  =  typename List::template map_accum_left_with< F::template apply_to, identity<B...> >;

    template< typename List, typename F, typename... B>
    using list_map_accum_right  =  typename List::template map_accum_right_with< F::template apply_to, identity<B...> >;


    template< typename List, typename... N >
    using list_drop  =  typename List::template drop< identity<N...>::eval() >;


    template< typename List, typename... N >
    using list_take  =  typename List::template take< identity<N...>::eval() >;


    template< typename List, typename... Cond >
    using list_drop_while  =  typename List::template drop_while< identity<Cond...>::template apply_to >;


    template< typename List, typename... Cond >
    using list_take_while  =  typename List::template take_while< identity<Cond...>::template apply_to >;


    template< typename List, typename... Less >
    using list_sort  =  typename List::template sort_with< identity<Less...>::template apply_to >;


    template< typename List, typename... Pred >
    using list_partition  =  typename List::template partition_with< identity<Pred...>::template apply_to >;


    template< typename List, typename... Pred >
    using list_split_by_first_occurence  =  typename List::template split_by_first_occurence_of< identity<Pred...>::template apply_to >;

}
