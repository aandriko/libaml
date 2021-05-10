#pragma once

#include "../head.hpp"
#include "../tail.hpp"

namespace aml::structure
{
    template<typename... X>
    using tail = typename aml::tail<X...>::template apply< aml::tail >
                             ::template < aml::head<X...>::template abstract_type >;
    // linker == head<X...>
}

        
