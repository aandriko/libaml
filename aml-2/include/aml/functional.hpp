/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "./logic.hpp"
#include "./object.hpp"
#include "./term_algebra.hpp"

namespace aml
{
    template<  template< typename... > class F
            ,  typename... N
            >
    struct curry
    {
        static_assert( sizeof...(N) != 0 );
    };


    template< template< typename... > class F
            , typename N
            >
    struct curry< F, N >
    {
    private:

        template< typename... X >
        struct bind_args
        {
            static_assert( sizeof...(X) == N::eval(),
                           "The number of input arguments for curried function is incorrect." );

            template<typename... Y>
            using apply_to  =  F<X..., Y...>;
        };

    public:

        template< typename... X >
        using apply_to = bind_args<X...>;
    };


    template<  template< typename... > class F  >
    struct curry<F, infinity>
    {
        //    private:
        template<typename... X>
        struct apply_to_
        {
        private:
            template<typename... no_args>
            struct nullary_function
            {
                static_assert( sizeof...(no_args) == 0 );
                using type = F<X...>;
            };

        public:
            template<typename... no_args>
            using apply_to = typename nullary_function<no_args...>::type;
        };

    public:

        template< typename... X >
        using apply_to  =   apply_to_< X... >;
    };


    template< template<typename...> class F
            , typename                    N
            , typename...                 M
            >
    struct curry< F, N, M... >
    {
        template< typename... X >
        using apply_to  =  curry<    curry<F, N>::
                                     template apply_to<X...>::
                                     template apply_to, M... >;
    };


    template< template<typename...> class F
            , typename... X
            >
    using curry_and_bind  =  typename curry< F, num<sizeof...(X)> >::template apply_to<X...>;


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


        template< typename X, typename >
        struct power_;


        template<typename X, typename N>
        struct power_intermediate_
        {
            using type = typename power_< typename X::type, typename N::down >::type;
        };


        template<typename X>
        struct power_terminate_
        {
            using type  =  X;
        };


        template< typename X,  typename N>
        struct power_
        {
            using type  =  typename bool_< N::eval() != 0 >::
                           template conditional<    power_intermediate_<X, N> ,
                                                    power_terminate_<X>    >::
                           type;
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


            using continue_  = bool_< ! is_same< decltype(next_step_<X>(nullptr, nullptr)), X >::eval() >;

            using type  =  typename continue_::template conditional< seq_step<X>, seq_terminate<X> >::type;
        };


    public:

        template<typename... X>
        using apply_to = typename is_one_parameter<X...>::type;


        template<typename X, typename... N>
        using power  =  typename conditional< is_same<infinity, typename is_one_parameter<N...>::type >,
                                              limit_<X>,
                                              power_<X, typename is_one_parameter<N... >::type > >::type;


        template<typename... X>
        using limit  =  typename limit_< typename is_one_parameter<X...>::type >::type;
    };


    template<typename... X>
    using identity  =  typename composition<>::template apply_to< X... >;


    template< typename T, typename... N >
    using power = typename composition<>::template power< T, identity<N...> >;


    template< typename... T >
    using limit  =  typename composition<>::template limit<T...>;

 
    template< template<typename... > class F >
    struct monoid
    {
       template<typename...>
        struct action;


        struct empty
        {
            using type = action<empty>;
            struct subterms
            {
                template< template<typename...> class >
                using apply = subterms;
            };
        };

 
        template<typename List, typename R = typename List::template apply<F>::template apply<action> >
        static R eternal_(decltype(nullptr), decltype(nullptr));


        template<typename List>
        static empty eternal_(decltype(nullptr), ... );

        template<typename... X>
        using eternal_action_on_F  =  decltype( eternal_<list<X...> >(nullptr, nullptr) );


        template< typename... X >
        struct action
        {
                        using type     =  action< F< X... > >;
            //            using type  = eternal_action_on_F<X...>;

            
            struct subterms
            {
                template< template<typename... > class G >
                using apply =  G<X...>;
            };
        };


        template<typename... N>
        struct power
        {
            static_assert(sizeof...(N) == 1);

            template< typename... X >
            using apply_to  =  typename aml::power< action< X... >, identity<N... > >::
                               subterms::
                               template apply< identity >;
        };
    };


}
