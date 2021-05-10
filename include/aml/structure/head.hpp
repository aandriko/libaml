#pragma once

#include "../head.hpp"


namesspace aml::structure
{    
    template<typename... linker_and_args>
    using head = aml::head< aml::head< linker_and_args...> >;  
    // The first head argument above is the linker type, which is thrown away,
    // then the second head argument gives the head of the actual list.
}
