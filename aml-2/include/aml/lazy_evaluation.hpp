#pragma once

#include "./logic.hpp"
#include "./functional.hpp"


namespace aml
{
    template<typename... T>
    struct hull
    {
        using type = identity<T...>;

        static void id() {};
    };


    template< auto... n >
    struct delay
    {
        static_assert( sizeof...(n) == 1 );
    };


    template< auto n >
    struct delay<n>
    {
        static_assert( n != 0, "A delay must be strictly positive. " );
    };


    template<>
    struct delay<>
    { };


    template<typename... >
    struct lazy_l;


    template< auto        n
            , typename    F
            , typename... X >
    struct lazy_l<  delay< n >,  F,  X...  >
    {
        using type = lazy_l<  delay< n-1 >,  F,  typename lazy_l< X >::type...  >;
    };


    template< typename    F
            , typename... X >
    struct lazy_l<  delay< 1 >,  F,  X...  >
    {
        using type = typename F::template apply_to< typename lazy_l< X >::type... >;
    };


    template< auto        n
            , typename    F
            , typename... X >
    struct lazy_l< lazy_l< delay< n >, F, X... > >
    {
        using type  =  typename lazy_l< delay<n>, F, X... >::type;
    };


    template< template<typename... > class F
              , typename... X
              >
    struct lazy_l<  F< X... >  >
    {
        using type  =  F< typename lazy_l<X>::type... >;
    };


    template< typename X >
    struct lazy_l< X >
    {
        using type  =  X;
    };


    template< auto n >
    struct evaluation_depth
    {
        template< typename... X >
        using evaluate  =  power< lazy_l<identity<X...>>, n >;
    };


    template<>
    struct evaluation_depth<0>
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
            using type = action< typename lazy_l<X>::type >;
            using read = X;
        };

        template< typename... X>
        using evaluate  =  typename limit< action<X...> >::read;
    };


    template< typename X
            , auto n = infinity
            >
    using evaluate  =  typename evaluation_depth< n >::template evaluate< X >;



    template<  typename X, typename N = infinity_t  >
    using evaluate_l  =  evaluate<X, N::eval() >;


    template<  template< typename... > class F
            ,  auto n = 1
            >
    struct lazy
    {
        template< typename... X >
        using apply_to = lazy_l<  delay<n>, curry<F>, X...  >;
    };
}
