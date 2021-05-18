#pragma once

#include <type_traits>

#include "./apply.hpp"
#include "./conslist.hpp"
#include "./parameters.hpp"
#include "./exponent.hpp"

namespace aml
{
    template<typename... X>
    using conditional =
        std::enable_if_t
        <
            sizeof...(X) == 3,                          
            std::conditional_t< conslist<X...>::head::eval(),
                                typename conslist<X...>::tail::head,
                                typename conslist<X...>::tail::tail::head >
        >;

    template<typename T>
    using add_type = typename T::type;
    
    template<bool b>
    struct bool_
    {
        static constexpr bool eval() { return b; }
    };
    
    
    template<template<typename...> class Pred>
    struct find    
    {
         // returns conslist<X>  with
        // X the first type in Args to satisfy Pred<X>::eval() == true.
        // If such a type X does not exist, then the empty list
        // conslist<> is returned.
        template<typename... X>
        struct in_
        {
            template<typename... Y>
            using this_template = in_<Y...>;
            
            using type =
                
                eval<
                typename
            std::conditional_t
            <
                sizeof...(X) == 0,
        
                    aml::delay< aml::delay< conslist<>  > >,

                aml::delay
                <
                    aml::apply
                    <
                        aml::conditional,
                        
                        apply<Pred,  apply<aml::head, conslist<X...> > >,
                        
                        delay< apply< conslist, apply<head, conslist<X...> > >>,
                        
                        delay
                        <
                            apply
                            <
                                add_type,
                                
                                apply
                                <
                                    fix_function< this_template >::template apply_to_args_in_conslist,
                            
                                    apply<tail, conslist<X...> >
                                >
                            >
                        >
                    >
                >
                    >::eval::eval::type>::type;
        };

        template<typename... X>
        using in = typename in_<X...>::type;
    };   
}
