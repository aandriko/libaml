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


    template<auto n>
    struct delay
    {
        static_assert( n != 0, "A delay must be strictly positive. " );
    };


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


    template< typename X
            , auto...  n
            >
    using evaluate  =  typename bool_<  hull< power<lazy_l<X>, n...> >::id
                                        ==
                                        hull< lazy_l<X > >::id  >::
                       template conditional< X,
                                             power<lazy_l<X>, n... > >;


    template<  typename X, typename... N  >
    using evaluate_l  =  evaluate<X, N::eval()... >;


    template<  template< typename... > class F
            ,  auto n = 1
            >
    struct lazy
    {
        template< typename... X >
        using apply_to = lazy_l<  delay<n>, curry<F>, X...  >;
    };
}
