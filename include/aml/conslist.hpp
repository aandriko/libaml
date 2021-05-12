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

        
        template<typename X>
        using cons = conslist<X>;

        
        template<typename X>
        using rcons = conslist<X>;


        template<template<typename...> class F>
        using apply = F<>;


        template<template<typename...> class, typename Z>
        struct fold_with
        {
            using on_the_right = Z;
            using on_the_left  = Z;
        };

        using reverse = conslist<>;

        static constexpr auto size() { return 0; }
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

        
        template<template<typename...> class F>
        using apply = F<H, T...>;

        
        template<template<typename...> class F, typename Z>  
        struct fold_with
        {
            using on_the_right = F<H, typename conslist<T...>::template fold_with<F, Z>::on_the_right >;
            // right fold: F<H, F< .... F<last, z> >... >

            using on_the_left = F< typename conslist<T...>::template fold_with<F, Z>::on_the_left, H>;
        };

        
        using reverse = typename tail::reverse::template rcons<H>;

        static auto constexpr size() { return 1 + sizeof...(T); }
    };

    
    template<typename... one_conslist_arg>
    using tail = typename conslist<one_conslist_arg...>::head::tail;
    // effect: tail<conslist<h, t...> > == conlist<t...> == conslist<h, t...>::tail
}
