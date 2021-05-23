#pragma once

#include "./exponent.hpp"

namespace aml
{
    template<int n, template<typename...> class F, typename... X>
    struct apply;

    
    template<typename... Term>
    struct evaluation;

    template<typename...>
    struct evaluation_;

    template<typename T>
    struct evaluation_<T> { using type = evaluation<T>; };


    template<typename T>
    struct evaluation_<evaluation<T> > { using type = evaluation<T>; };

    
    template<typename Term>
    struct evaluation<Term>
    {
        using term = Term;
        using type = typename evaluation_<term>::type;

    };
    
    
    template<template<typename...> class F, typename... X>
    struct evaluation<apply<0, F, X...>
    {
        using term = F< typename evaluation<X>::term... >;
        using type = typename evaluation_<term>::type;
    };


    template<int n, template<typename...> class F, typename... X>
    struct evaluation<apply<n, F, X...> >
    {
        using term = apply<n-1, F, typename evaluation<X>::term... >;
        using type = typename evaluation_<term>::type;
    };


    template<template<typename...> class F, typename... X>
    struct evaluation<F<X...> >
    {
        using term = F< typename evaluation<X>::term... >;
        using type = typename evaluation_<term>::type;
    };

    
    template<typename... Term>
    using eval = typename power<exp<infinity>, evaluation<Term...> >::term;
    
}       
