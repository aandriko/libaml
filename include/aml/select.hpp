#pragma once

#include "./parameters.hpp"
#include "./exponent.hpp"
#include "./conslist.hpp"

namespace aml
{
    template<auto... idx>
    struct select
    {
    public:
        template<typename... X>
        struct from
        {
            template<template<typename...> class Collector>
            using with_collector
            = Collector
                <
                     typename function::power<exp<idx>, tail>::template apply_to<conslist<X...>>::head...
                >;
        };
    };
}
