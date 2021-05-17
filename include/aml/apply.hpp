#pragma once


#include <type_traits>


namespace aml
{
    template<typename... >
    struct eval;

    
    template<int n, template<typename...> class F, typename... X>
    struct apply;

    
    template<typename Atomic_Term>
    struct eval<Atomic_Term>
    {
        using type = Atomic_Term;
    };


    template<template<typename...> class F, typename... X>
    struct eval<F<X...> >
    {
        using type = F<typename eval<X>::type... >;
    };

    
    template<typename T>
    struct eval<eval<T> >
    {
        using type = eval<typename eval<T>::type >;
    };
    

    template<int n, template<typename...> class F, typename... X>
    struct eval<apply<n, F, X...> >
    {
        using type = apply<n-1, F, X...>;
    };

    template<template<typename...> class F, typename... X>
    struct eval<apply<0, F, X...> >
    {
        using type = F< typename eval<X>::type... >;
    };
    
    
    template<template<typename...> class F, int n = 0>
    struct bra
    {
        template<typename... X>
        using ket = apply<n, F, X...>;
    };


    template<typename... X>
    struct ket
    {
        template<template<typename...> class F, int n = 0>
        using bra = apply<n, F, X...>;
    };
}       
