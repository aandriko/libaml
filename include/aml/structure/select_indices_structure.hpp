#pragma once

#include "../exponent.hpp"
#include "../basic_types.hpp"

namespace aml::structure
{
    template<typename...>
    struct array;

    template<typename Linker, typename... X>
    struct array<Linker, X...>
    {
        template<auto idx>
        using at = typename function<>::power<exp<idx>, tail>::template apply_to<conslist<X...>>::head;

        template<auto... idx>
        using sub_array = typename Linker::template abstract_type< at<idx>... >;
    };
}
