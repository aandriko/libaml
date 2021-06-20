#pragma once

namespace aml
{
    template< bool >
    struct bool_;

    using true_   =  bool_< true >;
    using false_  =  bool_< false >;


    template<>
    struct bool_< true >
    {
    private:

        struct check_emptyness_<typename... no_args>
        {
            static_assert( sizeof...(no_args) == 0,
                           "Too many arguments in conditional!" );

            template<typename X>
            struct return_
            {
                using type  =  X;
            };
        };


    public:

        using sfinae = true;


        template< typename T >
        using enable = T;


        template< typename    T
                , typename
                , typename... no_args
                >
        using conditional  =  typename check_emptyness_<no_args...>::
                              template return_<T>::type;


        static constexpr bool eval() { return true; };
    };


    template<>
    struct bool_< false >
    {
    private:

        struct check_emptyness_<typename... no_args>
        {
            static_assert( sizeof...(no_args) == 0,
                           "Too many arguments in conditional!" );

            template<typename X>
            struct return_
            {
                using type  =  X;
            };
        };


    public:

        template< typename
                , typename    F
                , typename... no_args
                >
        using conditional  =  typename check_emptyness_<no_args...>::
                              template return_<F>::type;


        static constexpr bool eval() { return false; };
    };


    template< typename  X >
    using as_bool = bool_< X::eval() >;


    template< typename    C
            , typename... X
            >
    using conditional  =  typename C::template conditional<X...>;


    template<typename Bool, typename... no_args>
    using not_  =  conditional< Bool, false_, true_, no_args... >;


    template< typename... Bool >
    using all   =  bool_< (       Bool::eval()  &&  ... ) >;


    template< typename... Bool >
    using any   =  bool_< (       Bool::eval()  ||  ... ) >;


    template< typename... Bool >
    using none  =  bool_< ( not_<Bool>::eval()  &&  ... ) >;


    template<template<typename...> class... Predicate>
    struct predicates
    {
        template<typename... X>
        using for_all  =  all< Predicate<X>... >;


        template<typename... X>
        using for_any  =  any< Predicate<X>... >;


        template<typename... X>
        using for_none  =  none< Predicate<X>... >;
    };


}
