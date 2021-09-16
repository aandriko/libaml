/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


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

        template< typename T
                , auto n_arguments
                >
        struct check_emptyness_
        {
            static_assert( n_arguments == 0,
                           "Too many arguments in conditional!" );

            using type = T;
        };


    public:

        template< typename T >
        using enable = T;


        using sfinae = enable<true_>;


        template< typename    T
                , typename
                , typename... no_args
                >
        using conditional  =  typename check_emptyness_< T, sizeof...(no_args) >::type;


        static constexpr bool eval() { return true; };
    };


    template<>
    struct bool_< false >
    {
    private:

       template< typename T
               , auto n_arguments
               >
        struct check_emptyness_
        {
            static_assert( n_arguments == 0,
                           "Too many arguments in conditional!" );

            using type = T;
        };

    public:

        template< typename
                , typename    F
                , typename... no_args
                >
        using conditional  =  typename check_emptyness_<F, sizeof...(no_args) >::type;


        static constexpr bool eval() { return false; };
    };


    template< typename    Condition
            , typename... X
            >
    using conditional  =  typename bool_<Condition::eval()>::template conditional<X...>;


    template<typename Bool, typename... no_args>
    using not_  =  conditional< Bool, false_, true_, no_args... >;


    template< typename... Bool >
    using all   =  bool_< (       Bool::eval()  &&  ... ) >;


    template< typename... Bool >
    using any   =  bool_< (       Bool::eval()  ||  ... ) >;


    template< typename... Bool >
    using none  =  bool_< ( not_<Bool>::eval()  &&  ... ) >;


    struct predicates
    {
    private:

        template<typename X, typename... no_args>
        struct identity_
        {
            static_assert(sizeof...(no_args) == 0);
            using type = X;
        };


        template< typename... X >
        using identity = typename identity_<X...>::type;

    public:

        template< template< typename... > class... Predicate >
        struct all
        {
            template<typename... X>
            using apply_to  =  bool_< (Predicate< identity< X... > >::eval() && ... ) >;
        };


        template< template< typename... > class... Predicate >
        struct any
        {
            template<typename... X>
            using apply_to  =  bool_< (Predicate< identity< X... > >::eval() || ... ) >;
        };


        template< template< typename... > class... Predicate >
        struct none
        {
            template<typename... X>
            using apply_to  = bool_< ! (Predicate< identity< X... > >::eval() || ... ) >;
        };
    };
}
