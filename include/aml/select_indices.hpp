/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "./exponent.hpp"
#include "./basic_types.hpp"

namespace aml
{
    template<auto... idx>
    struct select_indices
    {
        template<typename... X>
        using from = conslist< typename function<>::power<exp<idx>, tail>::template apply_to<conslist<X...>>::head... >;
    };


    template<typename... X>
    struct from
    {
        template<auto... idx>
        using select_indices = typename aml::select_indices<idx...>::template from<X...>;
    };
}
