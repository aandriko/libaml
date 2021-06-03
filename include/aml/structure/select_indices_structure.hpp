#pragma once

#include "./exponent.hpp"
#include "./basic_types.hpp"

namespace aml
{
    template<typename...>
    struct array;

    template<typename Linker, typename... X>
    struct array
    {
        template<auto idx>
        using at = typename function<>::power<exp<idx>, tail>::template apply_to<conslist<X...>>::head;

        template<auto... idx>
        using sub_array = typename Linker::template abstract_type< at<didx>... >;
    };
}
