#pragma once

#include "./nucleus.hpp"

namespace aml
{

    template<char... text>
    using string = object::parameters<text...>;
    
    template<typename Char=char, Char... txt> constexpr string<txt...>  operator ""_() { return { }; }
}
