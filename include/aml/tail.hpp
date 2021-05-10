#pragma once

namespace aml::lazy
{
    template<typename...>
    struct tail;

    
    template<typename H, typename... T>
    struct tail<H, T...>
    {
        template<template<typename...> class F>
        using apply_to = F<T...>;
    };

    
    template<>
    struct tail<>
    {
        template<template<typename...> class F>
        using apply = F<>;
    };    
}
