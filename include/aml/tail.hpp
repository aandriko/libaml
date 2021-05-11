#pragma once

namespace aml
{
    template<typename...>
    struct tail;


    template<>
    struct tail<> {};

    
    template<typename H, typename... T>
    struct tail<H, T...>
    {
        template<template<typename...> class F>
        using apply = F<T...>;
    };
    
}
