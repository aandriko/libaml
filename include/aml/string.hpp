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

    template<char... text>
    using string = object<>::list<text...>;

    template<typename Char=char, Char... txt> constexpr string<txt...>  operator ""_() { return { }; }
}
