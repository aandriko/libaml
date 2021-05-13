#pragma once

#include <type_traits>

#include "./conslist.hpp"

namespace aml::lazy
{
    template<typename...>
    struct join;
    
    template<template<typename... > class F, typename... X>
    struct join<F<X...> >
    {
        using type = F<X...>;
    };
    
    template<template<typename...> class F, typename... X, typename... Y, typename... Z>
    struct join<F<X...>, F<Y...>, Z... >
    {
        using type = typename join<F<X..., Y...>, Z... >::type;
    };
}


namespace aml
{
    template<typename... X>
    using join = typename lazy::join<X...>::type;


    template<typename...>
    struct partition;


    template<>
    struct partition<>
    {
    public:
        template<template<typename...> class>
        struct with
        {
            using accepted = partition<>;
            using rejected = partition<>;
        };
        
    private:
        template<typename...>
        friend struct partition;

        template<typename Accept, typename Reject>
        struct collector 
        {
            struct accept_
            {
                template<typename X>
                using add = collector<typename Accept::template rcons<X>, Reject>;
            };

            
            struct reject_
            {
                template<typename X>
                using add = collector<Accept, typename Reject::template rcons<X>>;
            };

            struct result
            {
                using accepted = typename Accept::template apply<partition>;
                using rejected = typename Reject::template apply<partition>;
            };
        };

        template<template<typename...> class Pred>
        struct fix_predicate
        {
            template<typename X, typename Collector>
            using scan_argument = typename
                std::conditional_t<Pred<X>::eval(),
                                   typename Collector::accept_,
                                   typename Collector::reject_>::template add<X>;
        };
    };       


    template<typename H, typename... T>
    struct partition<H, T...>
    {
        template<template<typename...> class Pred>
        using with =
            typename
            conslist<H, T...>::template rfold_with<
                                                      aml::partition<>::template fix_predicate<Pred>::
                                                      template scan_argument,

                                                      aml::partition<>::collector< conslist<>, conslist<> >

                                                  >::result;
    };      
}