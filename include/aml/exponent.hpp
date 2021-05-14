#pragma once

#include <type_traits>

#include "./conslist.hpp"


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


namespace aml::lazy
{
    template<typename... X>
    struct identity
    {
        static_assert(sizeof...(X) == 1, "The identity functor can only take exactly one parameter.");        
        using type = typename conslist<X... >::head;
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
        using type = typename power<exp<n-1>, typename T::type>::type;
    };

    
    template<typename T>
    struct power< aml::exp<aml::infinity>, T>
    {
    private:
        template<typename X, typename R = typename X::type>
        static constexpr R add_type_if_possible_(void*, void*);

        template<typename X>
        static constexpr X add_type_if_possible_(void*, ...);
        
        using next_T   = decltype(add_type_if_possible_<T>(nullptr, nullptr));
        using next_exp = std::conditional_t < std::is_same<T, next_T>::value, exp<0>, exp<infinity> >;

    public:
        using type = typename power<next_exp, next_T>::type;
    };
}


namespace aml
{    
    template<typename... X>
    using identity = typename aml::lazy::identity<X...>::type;


    template<typename... Exponent_and_Type>
    using power = typename aml::lazy::power<Exponent_and_Type...>::type;    

}


namespace aml::function
{
    template<typename N, template<typename...> class F>
    struct power;


    template<int n, template<typename...> class F>
    struct power<aml::exp<n>, F>
    {
        static_assert(n != 0, "");
        
        template<typename... X>
        using apply_to =
            typename power<aml::exp<n-1>, F>::template apply_to<F<X...> >;
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
        template<typename Z>
        struct state
        {
            using type = state<F<Z> >;
            using result = Z;
        };

        template<typename... X>
        using apply_to = typename aml::power< aml::exp<aml::infinity>, state< F<X...> > >::result;
    };
}
