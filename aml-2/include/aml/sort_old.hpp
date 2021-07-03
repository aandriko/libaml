#pragma once

namespace aml
{
    template< typename... >
    struct list;

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


        template<typename T>
        struct hull
        {
            using type = T;
        };


        template<int n, typename T>
        struct stabilizer
        {
            static constexpr int rk() { return n; }
            using type = T;
        };


        template<typename List, typename X>
        using apply_stabilizer = typename List::template rcons< stabilizer<List::size(), X> >;


        template<typename X, typename List>
        using remove_stabilizer = typename List::template cons<typename X::type>;


        template<template<typename...> class Less>
        struct fix_comparison
        {
            template<typename... Args>
            struct sort_non_empty_list;


            template<typename... Args>
            struct lazy_sort
            {
                using type = typename conditional
                                      <
                                          bool_<sizeof...(Args) == 0>,

                                          hull<list<>>,

                                          sort_non_empty_list<Args...>

                    >::type;
            };


            template<typename... Args>
            struct sort_non_empty_list
            {
                using pivot = typename list<Args...>::head;
                using tail  = typename list<Args...>::tail;

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

                using partition_t = typename tail::partition::template with<predicate>;

                /*
                using type = aml::join
                             <
                                 typename partition_t::accepted::template apply<lazy_sort>::type,

                                 list<pivot>,

                                 typename partition_t::rejected::template apply<lazy_sort>::type
                             >;
                */

                using type  =  typename partition_t::accepted::template rcons<pivot>::template apply<
                    partition_t::rejected::template cons>;

            };

            template<typename... Args>
            using sort = typename lazy_sort<Args...>::type;

        };
    };


    template<typename H, typename... T>
    struct sort<H, T...>
    {
        /*
        template<  template<typename...> class Less  >
        using with_   =  typename list< H, T... >::
            template lfold_with< sort<>::template apply_stabilizer, list<> >;
            //::template apply< sort<>::template fix_comparison<Less>::template sort >;
            */
        using stabilized = typename list<H, T...>::template lfold_with< sort<>::template apply_stabilizer, list<> >;

        template< template<typename...> class Less >
        using with  = typename  stabilized::template apply< sort<>::template fix_comparison<Less>::template sort >;
        
        /*
        template<template<typename...> class Less>
        using with = typename list<H, T...>::reverse

                                               ::template rfold_with
                                                          <
                                                              sort<>::template apply_stabilizer,

                                                              list<>

            >;
      
                                                ::reverse

                                                ::template apply< sort<>::template fix_comparison<Less>::template sort >

                                                ::template rfold_with<sort<>::template remove_stabilizer, list<>>;
        */
        ;

    };
}


#include "./list.hpp"
