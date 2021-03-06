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
    template<typename...>
    struct partition;


    template<>
    struct partition<>
    {
    public:
        template<template<typename...> class>
        struct with
        {
            using accepted = conslist<>;
            using rejected = conslist<>;
        };

    private:
        template<typename...>
        friend struct partition;

        template<typename Accept, typename Reject>
        struct collector
        {
            struct accept_
            {
                template<typename X>
                using add = collector<typename Accept::template cons<X>, Reject>;
            };


            struct reject_
            {
                template<typename X>
                using add = collector<Accept, typename Reject::template cons<X>>;
            };

            struct result
            {
                using accepted = Accept;
                using rejected = Reject;
            };
        };

        template<template<typename...> class Pred>
        struct fix_predicate
        {
            template<typename X, typename Collector>
            using scan_argument = typename
                conditional<Pred<X>,
                            typename Collector::accept_,
                            typename Collector::reject_>::template add<X>;
        };
    };


    template<typename H, typename... T>
    struct partition<H, T...>
    {
        template<template<typename...> class Pred>
        using with =
            typename
            conslist<H, T...>::template rfold_with
                                        <
                                            aml::partition<>::template fix_predicate<Pred>::
                                                              template scan_argument,

                                            aml::partition<>::collector< conslist<>, conslist<> >

                                         >::result;
    };
}
