#pragma once

#include "./object.hpp"
#include "./logic.hpp"
#include "./functional.hpp"


namespace aml
{
    template<typename... >
    struct lazy;


    template< auto        n
            , typename    F
            , typename... X >
    struct lazy<  num<n>,  F,  X...  >
    {
        using type = lazy<  num< n-1 >,  F,  typename lazy< X >::type...  >;
    };


    template< typename    F
            , typename... X >
    struct lazy<  num< 1 >,  F,  X...  >
    {
        using type = typename F::template apply_to< typename lazy< X >::type... >;
    };


    template< auto        n
            , typename    F
            , typename... X >
    struct lazy< lazy< num< n >, F, X... > >
    {
        using type  =  typename lazy< num< n >, F, X... >::type;
    };


    template< template<typename... > class F
              , typename... X
              >
    struct lazy<  F< X... >  >
    {
        using type  =  F< typename lazy<X>::type... >;
    };


    template< typename X >
    struct lazy< X >
    {
        using type  =  X;
    };


    template< typename... n >
    struct evaluation_depth
    {
        template< typename... X >
        using evaluate  =  power< lazy<identity<X...>>, identity<n...> >;
    };


    template<>
    struct evaluation_depth< num<0> >
    {
        template< typename... X >
        using evaluate = identity< X... >;
    };


    template<>
    struct evaluation_depth< infinity >
    {
        template<typename X>
        struct action
        {
            using type = action< typename lazy<X>::type >;
            using read = X;
        };

        template< typename... X>
        using evaluate  =  typename limit< action<X...> >::read;
    };


    template< typename X
            , typename n = infinity
            >
    using evaluate  =  typename evaluation_depth< n >::template evaluate< X >;


    template<  template<typename... > class F, auto n = 1 >
    struct lazy_function
    {
        template< typename... X >
        using apply_to  =  lazy< num<n>, typename curry< F, num<0> >::template apply_to<>, X... >;
    };
}
