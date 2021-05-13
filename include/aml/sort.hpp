#pragma once

#include <type_traits>

#include "./partition_and_join.hpp"
#include "./parameters.hpp"

namespace aml
{
    template<typename...>
    struct sort;
        

    template<>
    struct sort<>
    {
        template<template<typename...> class>
        using with = sort<>;

    private:
        template<typename...>
        friend struct sort;
        
        
        template<int n, typename T>
        struct stabilizer
        {
            static constexpr int rk() { return n; }
            using type = T;
        };

        
        template<typename X, typename ConsList>
        using apply_stabilizer = typename ConsList::template cons< stabilizer<ConsList::size(), X> >;

        
        template<typename X, typename ConsList>
        using remove_stabilizer = typename ConsList::template cons<typename X::type>;

        
        template<template<typename...> class Less>
        struct fix_comparison
        {
            template<typename... Args>
            struct sort_non_empty_list;

            
            template<typename... Args>
            struct lazy_sort
            {
                using type = typename std::conditional_t
                                      <
                                          sizeof...(Args) == 0,

                                          type::hull<conslist<>>,
                                                         
                                          sort_non_empty_list<Args...>

                    >::type;
            };
                       

            template<typename... Args>
            struct sort_non_empty_list
            {            
                using pivot = typename conslist<Args...>::head;
                using tail  = typename conslist<Args...>::tail;
                
                template<typename X>
                struct predicate
                {

                    static constexpr bool eval()
                    {
                        return

                            Less< typename X::type, typename pivot::type >::eval()
                            ||
                            (
                             ! Less< typename     X::type, typename pivot::type >::eval()  &&
                             ! Less< typename pivot::type, typename     X::type >::eval()  &&
                             X::rk() < pivot::rk()

                             );
                    };
                };

                using partition_t = typename tail::template apply<aml::partition>::template with<predicate>;

                
                using type = aml::join
                             <
                                 typename partition_t::accepted::template apply<lazy_sort>::type,

                                 conslist<pivot>,

                                 typename partition_t::rejected::template apply<lazy_sort>::type
                             >;
                
            };

            template<typename... Args>
            using sort = typename lazy_sort<Args...>::type;

        };
    };


    template<typename H, typename... T>
    struct sort<H, T...>
    {
        template<template<typename...> class Less>
        using with = typename conslist<H, T...>::reverse
            
                                               ::template rfold_with
                                                          <
                                                              sort<>::template apply_stabilizer,

                                                              conslist<>

                                                          >
                                                ::reverse
            
                                                ::template apply< sort<>::template fix_comparison<Less>::template sort >
                        
                                                ::template rfold_with<sort<>::template remove_stabilizer, conslist<>>;            
            
    }; 
}                      
