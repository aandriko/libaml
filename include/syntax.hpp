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


    template<template<typename...> class, typename...>
    struct instantiate
    {
        struct type {};
    };
   
    
    template<typename... Subterms>
    struct subterms
    {
        template<typename Function>
        using apply = typename Function::template apply_to<Subterms...>;
        
        template<template<typename...> class Function>
        using Apply = Function<Subterms...>;
        
        template<template<typename...> class Function>
        using Pointwise_apply = syntax::subterms< Function<Subterms>... >;	
        
        template<typename Function>
        using pointwise_apply = Pointwise_apply<Function::template apply_to>;
        
        template<typename... More_Subterms>
        using push_front = syntax::subterms<More_Subterms..., Subterms...>;
        
        template<typename... More_Subterms>
        using push_back = syntax::subterms<Subterms..., More_Subterms...>;	
    };

 	
    template<typename Atomic>
    struct term
    {
	using function = syntax::constant<Atomic>;

	template<typename...>
	using Function = Atomic;
	
	using subterms = syntax::subterms<>;
    };

    
    template<template<typename...> class F, typename... X>
    struct term<F<X...>>
    {
	using function = syntax::function<F>;

	template<typename... Y>
	using Function = F<Y...>;
	
	using subterms = syntax::subterms<X...>;
    };
    
}
