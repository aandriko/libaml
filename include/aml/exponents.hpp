#pragma once

#include <type_traits>


namespace aml::lazy
{
    template<typename... X>
    struct identity;

    template<typename X, typename... nothing>
    struct identity<X, nothing...>
    {
        static_assert(sizeof...(nothing) == 0, "The identity functor cannot take more than one parameter.");        
        using type = X;
    };
}


namespace aml
{    
    inline void infinity() { }

    
    template<auto n>
    struct exp
    {
        static_assert(std::is_integral<decltype(n)>::value ||
                      std::is_same< decltype(infinity), decltype(n) >::value, "");
    };    

    template<typename... X>
    using identity = typename lazy::identity<X...>::type;
}


namespace aml::type::dtl
{
    template<typename X, typename R = typename X::type>
    constexpr R add_type_if_possible_(void*, void*);

    template<typename X>
    constexpr X add_type_if_possible_(void*, ...);

    template<typename X>
    using if_possible_add_type_to = decltype( add_type_if_possible_<X>(nullptr, nullptr) );

    template<template<typename...> class F, typename X>
    struct state
    {
        using type   = state<F, F<X> >;
        using result = X;
    };
}


namespace aml::type::lazy
{
 
    template<typename N, typename>
    struct power;

    
    template<typename T>
    struct power<exp<0>, T>
    {
        using type = T;
    };

        
    template<int n, typename T>
    struct power<aml::exp<n>, T>
    {
        using type = power<exp<n-1>, typename T::type>;
    };

    
    template<typename T>
    struct power< aml::exp<aml::infinity>, T>
    {
    private:
        using next_T   = type::dtl::if_possible_add_type_to<T>;
        using next_exp = std::conditional_t < std::is_same<T, next_T>::value, exp<0>, exp<infinity> >;

    public:
        using type = typename power<next_exp, next_T>::type;
    };
}


namespace aml::type
{    
    template<typename N, typename T>
    using power = typename lazy::power<N, T>::type;    
}


namespace aml::function
{
    template<typename N, template<typename...> class F>
    struct power;


    template<int n, template<typename...> class F>
    struct power<aml::exp<n>, F>
    {
        template<typename... X>
        using apply_to = typename aml::type::lazy::power<exp<n-1>, aml::type::dtl::state<F, F<X...> > >::type::result;
    };


    template<template<typename...> class F>
    struct power<aml::exp<0>, F>
    {
        template<typename... X>
        using apply_to = aml::identity<X...>;
    };


    template<template<typename...> class F>
    struct power< aml::exp<aml::infinity>, F>
    {
        template<typename... X>
        using apply_to = typename type::lazy::power<aml::exp<aml::infinity>, aml::type::dtl::state<F, F<X...> > >::type::result;
    };
}
