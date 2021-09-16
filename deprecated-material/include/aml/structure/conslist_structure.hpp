/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "../conslist.hpp"


namespace aml::structure
{
        template<typename... X>
        struct list
        {
        private:
            using linker_t = aml::head<conslist<X...>>;
            using data_t   = typename conslist<X...>::tail;

            template<typename... Y>
            using list_ = list<Y...>;

        public:
            using head = typename data_t::head;


            using tail =
                typename data_t::tail::
                template cons<linker_t>::
                template apply<linker_t::template abstract_type>;


            using reverse = typename conslist<X...>::reverse::template apply<linker_t::template abstract_type>;


            template<template<typename...> class F>
            using apply = F<X...>;


            template<template<typename...> class F, typename Z>
            using lfold_with = typename data_t::template lfold_with<F, Z>::template apply<linker_t::template abstract_type>;

            template<template<typename...> class F, typename Z>
            using rfold_with = typename data_t::template rfold_with<F, Z>::template apply<linker_t::template abstract_type>;


            static constexpr auto size() { return data_t::size(); }
        };
}
