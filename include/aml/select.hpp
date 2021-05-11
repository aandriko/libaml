#pragma once

#include "./parameters.hpp"
#include "./exponent.hpp"
#include "./head.hpp"
#include "./tail.hpp"

namespace aml
{
    template<auto... idx>
    struct select
    {
    private:
        template<typename... X>
        using from_ = typename
            type::parameters
            <
                typename function::power<exp<idx+1>, tail >::template apply_to<X...>::template apply<head>...
            >;
    
    public:
        template<typename... X>
        using from = from_<void, X...>;
        
    };
}
