#pragma once

#include "./conslist.hpp"

namespace aml
{

    template<typename... T>
    struct enable
    {
    private:
        static_assert(sizeof...(T) == 1, "");

        template<bool b>
        struct if_aux_
        { 
            static_assert(b, "Enablement condition broken.");
            using type = typename conslist<T...>::head;
        };
       
    public:
        template<typename Bool>
        using if_ = typename if_aux_<Bool::eval()>::type;        
    };

    
    template<bool>
    struct bool_;

    using true_  = bool_<true>;
    using false_ = bool_<false>;
    
    
    template<>
    struct bool_<true>
    {
        static constexpr bool eval() { return true; }

        using sfinae = true_;

        template<typename... Alternatives>
        using conditional = typename
            enable< typename conslist<Alternatives...>::head >::
            template if_<bool_<sizeof...(Alternatives) == 2>>;
    };

    
    template<>
    struct bool_<false>
    {
        static constexpr bool eval() { return false; }


        template<typename... Alternatives>
        using conditional = typename
            enable< typename conslist<Alternatives...>::tail::head >::
            template if_<bool_<sizeof...(Alternatives) == 2> >;
    };

        
    template<typename... CTF>
    using conditional =
        typename conslist<CTF...>::tail::
        template apply
        <
            bool_< conslist<CTF...>::head::eval()>::template conditional
        >;


    template<typename T>
    using add_type = typename T::type;
        
}

namespace aml::type
{
    template<typename T>
    struct hull 
    {
        using type = T;
    };
}


namespace aml::function
{
    template<template<typename...> class F>
    struct hull
    {
        template<typename... X>
        using apply_to = F<X...>;
    };


    template<template<typename...> class...>
    struct parameters;
}


namespace aml::object
{
    template<auto o>
    struct hull 
    {
        static constexpr auto eval() { return o; }
    };
}


namespace aml::type
{
    template<typename... Type>
    struct parameters
    {
        template<template<typename...> class F>
        using apply = F<Type...>;
    };        
}


namespace aml::function
{
    template<template<typename...> class... Function>
    struct parameters
    {
        using as_types = type::parameters< hull<Function>... >;
    };
}


namespace aml::object
{
    template<auto... Object>
    struct parameters
    {
        using as_types = type::parameters< object::hull<Object>... >;

        
        template<template<auto...> class F>
        using apply = F<Object...>;
    };
}


namespace aml::type
{
    template<auto... Object>
    struct parameters< aml::object::hull<Object>... >
    {
        using as_objects = object::parameters<Object...>;

        
        template<template<typename...> class F>
        using apply = F< aml::object::hull<Object>... >;
    };


    template<template<typename...> class... Function>
    struct parameters< aml::function::hull<Function>... >
    {
        using as_functions = function::parameters<Function...>;

        template<template<typename...> class F>
        using apply = F< aml::function::hull<Function>... >;
    };
}

