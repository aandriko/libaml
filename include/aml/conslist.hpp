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


        template<template<typename...> class, typename Z>
        using rfold_with = Z;


        template<template<typename...> class, typename Z>
        using lfold_with = Z;
        
        
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

        
        using reverse = typename tail::reverse::template rcons<H>;

        
        template<template<typename...> class F>
        using apply = F<H, T...>;

        
        template<template<typename...> class F, typename Z>
        using rfold_with =
            F<H, typename conslist<T...>::template rfold_with<F, Z> >;

        
        template<template<typename...> class F, typename Z>
        using lfold_with =
            F<typename conslist<T...>::template lfold_with<F, Z>, H >;
        
        
        static auto constexpr size() { return 1 + sizeof...(T); }
    };

    
    template<typename... one_conslist_arg>
    using head = typename conslist<one_conslist_arg...>::head::head ;
    
    
    template<typename... one_conslist_arg>
    using tail = typename conslist<one_conslist_arg...>::head::tail;
    // effect: tail<conslist<h, t...> > == conlist<t...> == conslist<h, t...>::tail

    
    template<template<typename...> class F>
    struct fix_function
    {
    private:
        template<typename... Conslist>
        struct apply_to_args_in_conslist_
        {
            static_assert(sizeof...(Conslist) == 1, "The function arguments must be gathered in one conslist.");
            using type = typename aml::conslist<Conslist...>::head::template apply<F>;
        };

    public:            
        template<typename... Conslist>
        using apply_to_args_in_conslist = typename apply_to_args_in_conslist_<Conslist...>::type;
    };
}
