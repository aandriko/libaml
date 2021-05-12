#pragma once


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

        //        template<template<typename...> class F>
        //        using pointwise_apply_one = parameters<F<Type>...>;

        // template<template<typename...> class... F>
        // using pointwise_apply = parameters<F<Type>...>;
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

