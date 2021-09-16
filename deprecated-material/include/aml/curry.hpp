/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "./exponent.hpp"
#include "./conslist.hpp"
#include "./basic_types.hpp"
#include "./select_indices.hpp"


namespace aml
{
    template< int n
            , template<typename...> class F
            >
    struct curry
    {
    private:
        template<typename... X>
        struct apply_to_
        {
            static_assert(sizeof...(X) == n, "Wrong number of arguments for currying!");

            // Here we have:
            // curry<n, F>::apply_to<x1, ..., xn>::apply_to<y1, ... , yr> == F<x1, ... xn, y1, ... yr>
            template<typename... Y>
            using apply_to  =  F<X..., Y...>;

        };

    public:
        template<typename... X>
        using apply_to  =  apply_to_<X...>;
    };


    template< int n
            , template<typename...> class F
            , typename...                 X
            >
    using curry_bind  =  typename conslist<X...>::
                         template check< sizeof...(X) == n >::
                         template apply< curry<n, F>::template apply_to >;


    template< int n
            , template<typename...> class Curried_Function >
    struct uncurry
    {
        // Here we have:
        // uncurry<n, curried_f>::apply_to<x1, ..., xn, y1, ..., yr> == curried_f<x1,...,xn>::apply_to<y1,...,yr>.

    private:
        template< typename... Z >
        using x_args  =  typename function<>::
                         template power< exp< sizeof...(Z) - n >, tail >::
                         template apply_to< reverse< conslist< Z...> > >::
                         reverse;

        template< typename... Z >
        using y_args  =  typename function<>::
                         template power< exp< n >, tail >::
                         template apply_to< conslist< Z... > >;

    public:
        template< typename... Z >
        using apply_to = typename y_args< Z... >::
                         template apply< x_args< Z... >::template apply< Curried_Function >::template apply_to >;
    };


    template<template<typename...> class F, auto... n>
    struct rebind
    {
        template< typename... X >
        using apply_to = typename from<X...>::template select_indices<n...>::template apply<F>;
    };


    template<typename... X>
    using diagonal  =  conslist< identity<X...>, identity<X...> >;
}
