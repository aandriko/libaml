#pragma once


namespace aml::types
{
    template<typename T>
    struct hull 
    {
        using type = T;

        constexpr hull()                       = default;
        constexpr hull(hull const&)            = default;
        constexpr hull& operator=(hull const&) = default;
        constexpr hull& operator=(hull&&)      = default;
    }
}


namespace aml::objects
{
    template<auto o>
    struct hull 
    {
        static constexpr eval() { return o; }

        constexpr hull()                       = default;
        constexpr hull(hull const&)            = default;
        constexpr hull& operator=(hull const&) = default;
        constexpr hull& operator=(hull&&)      = default;
    }
}


namespace aml::types
{
    template<typename... Types>
    struct list
    {
        template<template<typename...> class F>
        using apply = F<Types...>;
    };        
}


namesapce aml::objects
{
    template<auto... Objects>
    struct list
    {
        using as_types = types::list< objects::hull<Objects>... >;
        
        template<template<auto...> class F>
        using apply = F<Objects...>;
    }
}


namespace aml::types
{
    template<auto... Objects>
    struct list< aml::objects::hull<Objects>... >
    {
        using as_objects = objects::list<Objects...>;

        
        template<template<typename...> class F>
        using apply = F< aml::objects::hull<Objects>... >;
    };
}

