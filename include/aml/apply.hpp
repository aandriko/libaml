#pragma once


namespace aml
{
    template<typename... >
    struct eval;

    
    template<typename... X>
    struct delay
    {
        using eval = aml::eval<X...>;
    };
    

    
    template<template<typename...> class F, typename... X>
    struct apply;

    
    template<typename Atomic_Term>
    struct eval<Atomic_Term>
    {
        using type = Atomic_Term;
    };


    template<typename Any_Term>
    struct eval<delay<Any_Term> >
    {
        using type = Any_Term;
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
    

    template<template<typename...> class F, typename... X>
    struct eval<apply<F, X...> >
    {
        using type = F< typename eval<X>::type...>;
    };
    
    
    template<template<typename...> class F>
    struct bra
    {
        template<typename... X>
        using ket = apply<F, X...>;
    };


    template<typename... X>
    struct ket
    {
        template<template<typename...> class F>
        using bra = apply<F, X...>;
    };
}       
