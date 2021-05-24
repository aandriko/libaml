#pragma once

#include "./apply.hpp"
#include "./exponent.hpp"
#include "./basic_types.hpp"

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

                eval
                <
                    aml::conditional
                    <
                        bool_<sizeof...(X) == 0>,
                
                        conslist<>,

                        apply
                        <
                            0,

                            aml::conditional,
                        
                            apply<0, Pred,  apply<0, aml::head, conslist<X...> > > ,
                        
                            apply<1, conslist, apply<1, head, conslist<X...> >  >,
                        
                            apply
                            <
                                1,

                                add_type,
                                
                                apply
                                <
                                    1,
                                    
                                    bra<this_template>::template ket,
                                    
                                    apply<1, tail, conslist<X...> >
                                >
                            >
                        >
                    >
                >;                        
        };

        template<typename... X>
        using in = typename in_<X...>::type;
    };   
}
