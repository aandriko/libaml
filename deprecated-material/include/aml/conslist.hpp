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
    template<typename...>
    struct conslist;


    template<>
    struct conslist<>
    {
        template<typename X>
        using cons = conslist<X>;


        template<typename X>
        using rcons = conslist<X>;


        using reverse = conslist<>;


        template<template<typename...> class F>
        using apply = F<>;


        template<template<typename...> class>
        using pointwise_apply = conslist<>;


        template<template<typename...> class, typename Z>
        using rfold_with = Z;


        template<template<typename...> class, typename Z>
        using lfold_with = Z;


        static constexpr auto size() { return 0; }


    private:
        template<typename... X, typename... Y>
        static conslist<X..., Y...> combine_(conslist<X...>*, conslist<Y...>*);


        template<typename List_1, typename List_2>
        using combine = decltype(combine_(static_cast<List_1*>(nullptr), static_cast<List_2*>(nullptr)));


        template<typename... Lists>
        struct join_
        {
            using type = typename conslist<Lists...>::template rfold_with<combine, conslist<> >;
        };

        template<bool b>
        struct check_
        {
            static_assert(b, "");
            using type = conslist<>;
        };

    public:
        template<typename... Lists>
        using join = typename join_<Lists...>::type;

        template<bool b>
        using check = typename check_<b>::type;
    };


    template<typename H, typename... T>
    struct conslist<H, T...>
    {
        template<typename X>
        using cons = conslist<X, H, T...>;


        template<typename X>
        using rcons = conslist<H, T..., X>;


        using head = H;
        using tail = conslist<T...>;


        using reverse = typename tail::reverse::template rcons<H>;


        template<template<typename...> class F>
        using apply = F<H, T...>;


        template<template<typename...> class F>
        using pointwise_apply = conslist<F<H>, F<T>... >;


        template<template<typename...> class F, typename Z>
        using rfold_with =
            F<H, typename conslist<T...>::template rfold_with<F, Z> >;


        template<template<typename...> class F, typename Z>
        using lfold_with =  F<Z, typename conslist<T...>::template lfold_with<F, H> >;


        static auto constexpr size() { return 1 + sizeof...(T); }


    private:
        template<bool b>
        struct check_
        {
            static_assert(b, "");
            using type = conslist<H, T...>;
        };

    public:
        template<bool b>
        using check = typename check_<b>::type;

    };


    template<typename... one_conslist_arg>
    using head = typename conslist<one_conslist_arg...>::head::head ;


    template<typename... one_conslist_arg>
    using tail = typename conslist<one_conslist_arg...>::head::tail;
    // effect: tail<conslist<h, t...> > == conlist<t...> == conslist<h, t...>::tail


    template<typename... list_and_params>
    using cons = typename conslist<list_and_params...>::tail::template apply< conslist<list_and_params...>::head::template cons>;


    template<typename... list_and_params>
    using rcons = typename conslist<list_and_params...>::tail::template apply< conslist<list_and_params...>::head::template rcons>;

    template<typename... list>
    using reverse = typename conslist<list...>::template check<conslist<list...>::size() == 1>::head::reverse;


    template<typename... Conslists>
    using join = typename conslist<>::template join<Conslists...>;


    template<typename... List_Function_Terminal>
    using lfold =
        typename conslist<List_Function_Terminal...>::
        template check< conslist<List_Function_Terminal...>::size() == 3>::head::
        template lfold_with
                 <
                     conslist<List_Function_Terminal...>::tail::head::template apply_to,
                     typename conslist<List_Function_Terminal...>::tail::tail::head
                 >;


    template<typename... List_Function_Terminal>
    using rfold =
        typename conslist<List_Function_Terminal...>::
        template check< conslist<List_Function_Terminal...>::size() == 3>::head::
        template rfold_with
                 <
                     conslist<List_Function_Terminal...>::tail::head::template apply_to,
                     typename conslist<List_Function_Terminal...>::tail::tail::head
                 >;


    template<typename... X>
    struct size
    {
        static constexpr auto eval() { return sizeof...(X); };
    };
}
