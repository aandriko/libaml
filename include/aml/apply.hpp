#pragma once

#include "exponent.hpp"

namespace aml
{
    template<int n, template<typename...> class F, typename... X>
    struct apply;


    template<int n, template<typename...> class F>
    struct apply<n, F>
    {
        template<typename... X>
        using with_arguments = apply<n, F, X...>;
    };


    template<typename... Term>
    struct evaluation;


    template<typename Term>
    struct evaluation<Term>
    {
        using term = Term;
        using type = evaluation<Term>;
    };


    template<typename Term>
    struct evaluation<evaluation<Term> >
    {
        using term = Term;
        using type = evaluation<Term>;
    };


    template<template<typename...> class F, typename... X>
    struct evaluation< apply<0, F, X...> >
    {
        using term = F<typename evaluation<X>::term... >;
        using type = evaluation<term>;
    };


    template<int n, template<typename...> class F, typename... X>
    struct evaluation<apply<n, F, X...> >
    {
        using term = apply<n-1, F, typename evaluation<X>::term... >;
        using type = evaluation<term>;
    };


    template<template<typename...> class F, typename... X>
    struct evaluation<F<X...> >
    {
        using term = F< typename evaluation<X>::term... >;
        using type = evaluation<term>;
    };


    template<auto n>
    struct exp;


    template<typename...>
    struct pwer;


    template<typename... Term>
    using eval = typename power<exp<infinity>, evaluation<Term...> >::term;


    template<typename... X>
    using application = typename conslist<X...>::tail::tail::template
                         apply
                         <
                             apply
                             <
                                 conslist<X...>::head::eval(),
                                 conslist<X...>::tail::head::template apply_to

                             >::template with_arguments
                          >;
}
