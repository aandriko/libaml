#pragma once

#include "./basic_types.hpp"

namespace aml
{

    template<char... text>
    using string = object<>::list<text...>;
    
    template<typename Char=char, Char... txt> constexpr string<txt...>  operator ""_() { return { }; }
}
