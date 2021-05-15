#pragma once

#include <type_traits>

#include "./conslist.hpp"
#include "./parameters.hpp"

namespace aml
{
    template<template<typename...> class Pred>
    struct find    
    {
    private:
        template<typename... >
        struct in_;

        
        template<typename... Args>
        struct in_ht_
        {
            using head = typename conslist<Args...>::head;
            using tail = typename conslist<Args...>::tail;
            
            using type = typename
                         std::conditional_t
                         <
                             Pred<head>::eval(),

                             type::hull<conslist<head>>,

                             typename tail::template apply<in_>

                         >::type;                       
        };

        
        template<typename... Args>
        struct in_
        {
            using type = typename               
                         std::conditional_t
                         <
                             sizeof...(Args) == 0,

                             type::hull<conslist<>>,

                             in_ht_<Args...>

                         >::type;
        };
            
        public:

            template<typename... Args>
            using in = typename in_<Args...>::type;  // returns conslist<X>  with
                                                     // X the first type in Args to satisfy Pred<X>::eval() == true.
                                                     // If such a type X does not exist, then the empty list
                                                     // conslist<> is returned.
        };
}
