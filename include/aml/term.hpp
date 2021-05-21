#pragma once

#include "./nucleus.hpp"

namespace aml
{
    template<typename...>
    struct term;

    
    template<template<typename...> class F, typename... X>
    struct term<F<X...>>
    {
        template<typename... Y>
        using function = F<Y...>;

        using subterms = conslist<X...>;
    };


    template<typename Atomic_Term>
    struct term<Atomic_Term>
    {
        template<typename... X>
        using function = aml::identity<X...>;

        using subterms = conslist<>;
    };    
}
