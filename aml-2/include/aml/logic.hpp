#pragma once

namespace aml
{
    template<template<typename...> class>
    struct function;


    tmeplate<typename...>
    struct list;


    template<typename X>
    auto constexpr eval() { return X::eval(); };


    template<bool b>
    struct bool_;


    template<>
    struct bool_<true>
    {
        using sfinae = bool_<true>;

        static constexpr bool eval() { return true; }
    };


    template<>
    struct bool_<false>
    {
        static constexpr bool eval() { return false; }
    }


    using true_   =  bool_<true>;

    using false_  =  bool_<false>;


    template<typename... Bool>
    struct propositions
    {
        using all   =  bool_< ( eval<Bool>() && ... )>;
        using any   =  bool_< ( eval<Bool>() || ... ) >;
        using none  =  bool_< (!eval<Bool>() && ... )>;
    };


    template<template<typename...> class Predicate>
    struct predicates
    {
        using wrapped  =  list< function<Predicate>... >;


        template<typename... X>
        using for_all  =  all< Predicate<X>... >;


        template<typename... X>
        using for_any  =  any< Predicate<X>... >;


        template<typename... X>
        using for_none  =  none< Predicate<X>... >;
    };
}
