#pragma once


namespace aml
{
    template<typename...>
    struct conslist;

    
    template<>
    struct conslist<>
    {
        template<template<typename...> class F>
        using apply = F<>;
    };


    template<typename H, typename... T>
    struct conslist<H, T...>
    {
        using head = H;
        using tail = conslist<T...>;

        template<template<typename...> class F>
        using apply = F<H, T...>;
    };

    
    template<typename... one_conslist_arg>
    using tail = typename conslist<one_conslist_arg...>::head::tail;
    // effect: tail<conslist<h, t...> > == conlist<t...> == conslist<h, t...>::tail
}
