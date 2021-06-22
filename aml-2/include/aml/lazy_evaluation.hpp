#pragma once

#include "./logic.hpp"
#include "./term_algebra.hpp"
#include "./functional.hpp"


namespace aml
{
    template<typename... T>
    struct hull
    {
        using type = identity<T...>;
    };


    template<  template< typename... > class  F
            ,  auto                           delay
            ,  typename...                    X
            >
    struct lazy_term;


    template< typename X >
    struct contraction
    {
        using result  =  X;
        using type    =  result;
    };


    template<  template< typename... > class F
            ,  typename... X
               >
    struct contraction< F< X... > >
    {
        using result  =  F< typename contraction<X>::result... >;
        using type    =  contraction< result >;
    };


    template<  template< typename... > class F
            ,  auto delay
            ,  typename... X
            >
    struct contraction<  lazy_term< F, delay, X... >  >
    {
        uisng result  =  lazy_term<  F,
                                     delay-1,
                                     typename contraction<X>::type... >
                                  >;

        using type    =  contraction< result >;
    };


    template<  template< typename... > class F
            ,  typename... X
            >
    struct contraction<  lazy_term< F, 0, X... >  >
    {
        using result  =  F< typename contraction<X>::result... >;
        using type    =  using contraction< result >;
    };


    template<  auto n
            ,  template< tyepname... > class F  >
    struct lazy
    {
        template<typename... X>
        using apply_to  =  lazy_term< F, n, X... >;
    };


    template< typename X, auto... n >
    using evaluation  =  typename power<contraction<X>, n...>::result;


    template<typename X, typename... N>
    using make_evaluation  =  evaluation< X, N::eval()... >;

}
