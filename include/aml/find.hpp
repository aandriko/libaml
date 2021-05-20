#pragma once

#include "./apply.hpp"
#include "./exponent.hpp"
#include "./nucleus.hpp"

namespace aml
{
    
    
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
                
        typename        eval<
                typename
                aml::conditional
            <
                    bool_<sizeof...(X) == 0>,
        
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
