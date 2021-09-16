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
    template<  template< typename... > class F  >
    struct function
    {
        template< typename... X >
        using apply_to  =  F< X... >;
    };


    template< typename... Subterms >
    struct subterms;


    template<>
    struct subterms<>
    {
        template<  template< typename... > class F  >
        using apply = F< >;

        static auto constexpr size() { return 0; }
    };


    template< typename    H
            , typename... T >
    struct subterms< H, T... >
    {
        using head  =  H;
        using tail  =  subterms< T... >;

        template<  template< typename... > class F  >
        using apply = F< H, T... >;

        static auto constexpr size() { return 0; }
    };


    template< typename    T
            , typename... no_args
            >
    struct term
    {
    private:
        static_assert(sizeof...(no_args) == 0, "term<> takes only one argument!");

        template<typename Y, typename... no_args_>
        struct identity_
        {
            static_assert(sizeof...(no_args_) == 0,
                          "The identity function takes exactly one argument. ");

            using type  =  Y;
        };

    public:

        template<typename... Y>
        using function  =  typename identity_<Y...>::type;

        using subterms  = aml::subterms<T>;
    };


    template<  template< typename...> class F
            ,  typename... X
            >
    struct term<  F< X... >  >
    {
        template<typename... Y>
        using function  =  F< Y... >;

        using subterms  =  aml::subterms< X... >;
    };


    template< typename... X >
    struct is_same;


    template<>
    struct is_same<>
    {
        static constexpr bool eval() { return true; }
    };


    template< typename X >
    struct is_same<X>
    {
        static constexpr bool eval() { return true; }
    };


    template< typename    X
            , typename    Not_X
            , typename... Z
            >
    struct is_same< X, Not_X, Z... >
    {
    public:

        static constexpr bool eval()
        {
            return false;
        }
    };


    template< typename    X
            , typename... Z
            >
    struct is_same< X, X, Z... >
    {
    public:

        static constexpr bool eval()
        {
            return is_same<X, Z...>::eval();
        }
    };

}
