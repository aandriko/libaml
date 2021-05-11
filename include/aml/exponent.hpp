#pragma once

#include <type_traits>

#include "./head.hpp"


namespace aml
{    
    inline void infinity() { }

    
    template<auto n>
    struct exp
    {
        static_assert(std::is_integral<decltype(n)>::value ||
                      std::is_same< decltype(infinity), decltype(n) >::value, "");
    };    


}


namespace aml::dtl
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


namespace aml::lazy
{
    template<typename... X>
    struct identity
    {
        static_assert(sizeof...(X) == 1, "The identity functor can only take exactly one parameter.");        
        using type = typename head<X...>::type;
    };


    template<typename...>
    struct power;

    // power<exp<0>, T> == T
    //
    // power<exp<n>, T> == T::type .... ::type    (n-times type)
    //
    // power<exp<infinty>, T> == power<exp<n>, T>,
    // with n chosen to satisfy one of the following condtions A, B:
    //   A: power<exp<n>, T>::type == power<exp<n>, T>
    //   or
    //   B: power<exp<n>, T>::type does not exist.
    
    
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
        using next_T   = dtl::if_possible_add_type_to<T>;
        using next_exp = std::conditional_t < std::is_same<T, next_T>::value, exp<0>, exp<infinity> >;

    public:
        using type = typename power<next_exp, next_T>::type;
    };
}


namespace aml
{    
    template<typename... X>
    using identity = typename lazy::identity<X...>::type;


    template<typename... Exponent_and_Type>
    using power = typename lazy::power<Exponent_and_Type...>::type;    
}


namespace aml::function
{
    template<typename N, template<typename...> class F>
    struct power;


    template<int n, template<typename...> class F>
    struct power<aml::exp<n>, F>
    {
        template<typename... X>
        using apply_to = typename
            aml::lazy::power
            <
                exp<n-1>,
            
                aml::dtl::state<F, F<X...> >
            
            >::type::result;
    };


    template<template<typename...> class F>
    struct power<aml::exp<1>, F>
    {
        template<typename... X>
        using apply_to = F<X...>;
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
        using apply_to = typename lazy::power<aml::exp<aml::infinity>, aml::dtl::state<F, F<X...> > >::type::result;
    };
}