#pragma once

#include "./exponent.hpp"
#include "./nucleus.hpp"

namespace aml
{
    template<auto... idx>
    struct select_indices
    {
        template<typename... X>
        using from = conslist< typename function::power<exp<idx>, tail>::template apply_to<conslist<X...>>::head... >;
    };


    template<typename... X>
    struct from
    {
        template<auto... idx>
        using select_indices = typename aml::select_indices<idx...>::template from<X...>;
    };
}
