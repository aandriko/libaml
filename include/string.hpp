#pragma once

#include "types_and_objects.hpp"

namespace aml
{

    template<char... text>
    using string = objects::list<text...>;
    
    template<typename Char=char, Char... txt> constexpr string<txt...>  operator ""_() { return { }; }
}
