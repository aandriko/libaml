/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "./apply.hpp"
#include "./exponent.hpp"
#include "./basic_types.hpp"

namespace aml
{


    template<template<typename...> class Pred>
    struct find
    {
    private:
        template<typename... X>
        struct in_
        {
            template<typename... Y>
            using this_template = in_<Y...>;

            using type  =  eval
                           <
                               aml::conditional
                               <
                                   bool_<sizeof...(X) == 0>,    conslist<>,

                                   apply
                                   <
                                       0,


                                       aml::conditional,


                                       apply
                                       <
                                           0,    Pred,    apply
                                                          <
                                                              0,    aml::head,    conslist<X...>
                                                          >
                                       >,

                                       apply
                                       <
                                           1,    conslist,    apply
                                                              <
                                                                  1,    head,    conslist<X...>
                                                              >
                                       >,


                                       apply
                                       <
                                           1,    add_type,    apply
                                                              <
                                                                  1,    bra<this_template>::template ket,    apply
                                                                                                             <
                                                                                                                 1, tail, conslist<X...>
                                                                                                             >
                                                              >
                                       >
                                   >
                               >
                           >;
        };

    public:
         // returns conslist<X>  with
        // X the first type in Args to satisfy Pred<X>::eval() == true.
        // If such a type X does not exist, then the empty list
        // conslist<> is returned.
        template<typename... X>
        using in = typename in_<X...>::type;
    };
}
