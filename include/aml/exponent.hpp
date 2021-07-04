/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "./basic_types.hpp"


namespace aml
{
    inline void infinity() { }


    template<auto n>
    struct exp;


    template<>
    struct exp<0>
    {
        template<typename T>
        using power = T;

        static constexpr auto eval() { return 0; }
    };


    template<int n>
    struct exp<n>
    {
    private:
        template<typename... T>
        using T_ = typename enable<typename conslist<T...>::head>::template if_<bool_<sizeof...(T) == 1> >;

    public:
        template<typename... T>
        using power = typename exp<n-1>::template power< typename T_<T...>::type >;

        static constexpr auto eval() { return n; }
    };


    template<>
    struct exp<infinity>
    {
    private:
        template<auto>
        friend struct exp;

        template<typename X, typename R = typename X::type>
        static constexpr R add_type_if_possible_(void*, void*);

        template<typename X>
        static constexpr X add_type_if_possible_(void*, ...);

        template<typename... T>
        using T_ = typename enable< typename conslist<T...>::head >::template if_<bool_<sizeof...(T) == 1>>;

        template<typename T>
        using next_T   = decltype(add_type_if_possible_<T>(nullptr, nullptr));

        template<typename T>
        using next_exp = conditional < is_same<T, next_T<T>>, exp<0>, exp<infinity> >;

        template<typename... T>
        struct power_
        {
            using type = typename next_exp< T_<T...> >::template power< next_T<T_<T...> > >;
        };

    public:
        template<typename... T>
        using power =  typename power_<T...>::type;

        static constexpr auto eval() { return infinity; }
    };


    template<typename... Exponent_and_Type>
    using power = typename
        enable
        <
            typename conslist<Exponent_and_Type...>::head::

            template power<typename conslist<Exponent_and_Type...>::tail::head > 

        >::template
        if_< bool_<sizeof...(Exponent_and_Type) == 2> >;
}


namespace aml
{
    template<template<typename...> class...>
    struct function;


    template<>
    struct function<>
    {
        template<typename Exp, template<typename...> class F>
        struct power
        {
        private:
            template<typename... Z>
            struct state
            {
                using type   = state<F<Z...> >;
                using result = typename conslist<Z...>::head;
            };

            template<typename... Z>
            using apply_to_ = typename aml::power< exp<Exp::eval()-1>, state<Z...> >::type::result;

        public:
            template<typename... X>
            using apply_to =
                typename
                bool_<Exp::eval() == 0>::template conditional
                                                  <
                                                      aml::function<identity>,

                                                      aml::function<apply_to_>

                                                  >::template apply_to<X...>;
        };
    };
}
