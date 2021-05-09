#pragma once

#include "../string.hpp"
#include "../parameters.hpp"


namespace aml::adt
{
    template<typename...>
    struct list;


    template<typename Linker, typename H, typename... T>
    struct list<Linker, H, T...>
    {
        using head = H;
        using tail = typename Linker::template subtype<decltype("list"_), T...>;

        template<template<typename...> class F>
        using apply = F<H, T...>;

        template<template<typename...> class F>
        using pointwise_apply = list<Linker, F<H>, F<T>... >;
    };

    
    template<typename Linker>
    struct list<Linker>
    {
        using tail = list<Linker>;

        template<template<typename...> class F>
        using apply = F<>;

        template<template<typename...> class>
        using pointwise_apply = list<Linker>;
    };
}
