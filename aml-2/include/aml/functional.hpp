#pragma once

#include "./logic.hpp"

namespace aml
{
    template<  template< typename... > class F
            ,  auto...  n
            >
    struct curry;


    template< template< typename... > class F
            , auto n
            >
    struct curry< F, n >
    {
    private:

        template< typename... X >
        struct bind_args
        {
            static_assert( sizeof...(X) == n,
                           "The number of input arguments for curried function is incorrect." );

            template<typename... Y>
            using apply_to  =  F<X..., Y...>;
        };

    public:

        template< typename... X >
        using apply_to = bind_args<X...>;
    };


    // This is curry<F, \infinity>:
    template<  template< typename... > class F  >
    struct curry<F>
    {
        template<typename... X>
        using apply_to = F< X... >;
    };


    template< template<typename...> class F
            , auto    n
            , auto... m
            >
    struct curry< F, n, m... >
    {
        template< typename... X >
        using apply_to  =  curry<    curry<F, n>::
                                     template apply_to<X...>::
                                     template apply_to,          m...    >;
    };


    template< template<typename...> class F
            , typename... X
            >
    using curry_and_bind  =  typename curry< F, sizeof...(X) >::template apply_to<X...>;

    template<typename F, typename... N>
    using make_currying = curry< F::template apply_to, N::eval()... >;


    template< template<typename... > class... >
    struct composition;


    template<  template<typename...> class    G
            ,  template<typename...> class... F
            >
    struct composition< G, F... >
    {
        template< typename... X >
        using apply_to  =  G<  typename composition< F... >::template apply_to< X... >  >;
    };


    template<  template<typename...> class  F  >
    struct composition< F >
    {
        template< typename... X >
        using apply_to  =  F< X... >;
    };

    // identity
    template<>
    struct composition<>
    {
    private:
        template< typename X, typename... no_args >
        struct is_one_parameter
        {
            static_assert(sizeof...(no_args) == 0,
                          "composition<> == identity takes only one parameter!");

            using type = X;
        };


        template< typename X, auto n >
        struct power_;


        struct power_intermediate_
        {
            template<typename X, auto n>
            using type = typename power_< typename X::type, n-1 >::type;
        };


        struct power_terminate_
        {
            template<typename X, auto >
            using type  =  X;
        };


        template< typename X, auto n>
        struct power_
        {
            using type  =  typename bool_< n!= 0 >::
                           template conditional< power_intermediate_, power_terminate_ >::
                           template type<X, n>;
        };


        template< typename >
        struct limit_;


        template<typename T>
        struct seq_step
        {
            using type  =  typename limit_< typename T::type >::type;
        };


        template<typename T>
        struct seq_terminate
        {
            using type  =  T;
        };


        template<typename X>
        struct limit_
        {
            template< typename T
                    , typename R = typename T::type
                    >
            static constexpr R next_step_( decltype(nullptr), decltype(nullptr) );


            template< typename T >
            static constexpr T next_step_( decltype(nullptr), ... );

            template<typename>
            static constexpr void term_id() { };

            using continue_  =  bool_< term_id<decltype( next_step_<X>(nullptr, nullptr) )>  !=  term_id<X> >;
            using type  =  typename continue_::template conditional< seq_step<X>, seq_terminate<X> >::type;
        };


    public:

        template<typename... X>
        using apply_to = typename is_one_parameter<X...>::type;

        template<typename X, auto n>
        using power  =  typename power_<X, n>::type;

        template<typename... X>
        using limit  =  typename limit_< typename is_one_parameter<X...>::type >::type;
    };


    template<typename... X>
    using identity  =  typename composition<>::template apply_to< X... >;


    template< typename T, auto n >
    using power = typename composition<>::template power< T, n >;


    template< typename... T >
    using limit  =  typename composition<>::template limit<T...>;


    template< typename X, typename... N>
    using make_power  =  power< X, identity< N... >::eval()  >;



    template< template<typename... > class F >
    struct monoid
    {
        template< typename... X >
        struct action
        {
            using type  =  action< F< X... > >;
            using read  =  identity< X... >;
        };

        template<auto n, typename... X>
        using power = typename aml::power< action<X...>, n >::read;

        template<auto n>
        struct fix_exponent
        {
            static constexpr auto eval() { return n; };

            template< typename... X>
            using power  =  typename monoid<F>::power<n, X...>;
        };


        template< typename... X >
        using limit  =  typename limit< action<X...> >::read;
    };
}
