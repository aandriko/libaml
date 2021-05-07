#pragma once

namespace aml::syntax
{
    template<template<typename...> class Function>
    struct function
    {	
	template<typename... Subterms>
	using apply_to = Function<Subterms...>;
    };

    
    template<typename... C>
    struct constant;

    
    template<typename C>
    struct constant<C>
    {
	template<typename...>
	using apply_to = C;	
    };


    template<typename... Subterms>
    struct subterms
    {
        template<template<typename...> class F>
        using apply = F<Subterms...>;
    };

 	
    template<typename Atomic>
    struct term
    {
	template<typename...>
	using function = Atomic;
	
	using subterms = syntax::subterms<>;
    };

    
    template<template<typename...> class F, typename... X>
    struct term<F<X...>>
    {
	template<typename... Y>
	using function = F<Y...>;
	
	using subterms = syntax::subterms<X...>;
    };
    
}
