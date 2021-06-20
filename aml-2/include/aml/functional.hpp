#pragma once

#include "./logic.hpp"
#include "./term_algebra.hpp"

namespace aml
{
    template<  template< typename... > class F
            ,  auto n...
            >
    struct curry;


    template<  template< typename... > class F  >
    struct curry< F, n >
    {
    private:

        template< typename... X >
        struct apply_F
        {
            static_assert( sizeof...(X) == n,
                           "The number of input arguments for curried function is incorrect." );

            template<typename... Y>
            using apply_to  =  F<X..., Y...>;
        }

    public:

        template< typename... X >
        using apply_to = apply_F<  X... >::type;;
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
        template<typename... X>
        using apply_to  =  typename curry<    curry< F, m... >::template apply_to, n    >::
                           template apply_to< X... >;
    };


    template< template<typename...> class F
            , typename... X
            >
    using curry_and_bind  =  typename curry< F, sizeof...(X) >::template apply_to<X...>;


    template< template<typename... > class... >
    struct composition;


    template<  template<typename...> class    G,
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
        using apply_to  =  F< X... >
    };

    // identity
    template<>
    struct composition<>
    {
        template< typename X, typename... no_args >
        struct is_one_parameter
        {
            static_assert(sizeof...(no_args) == 0,
                          "composition<> == identity takes only one parameter!");

            using type = X;
        };

        template<typename... X>
        using apply_to = typename is_one_parameter<X...>::type;
    };


    template<typename... X>
    using identity  =  typename composition<>::template apply_to< X... >;


    template< auto n >
    struct idx
    {
        static constexpr auto eval() { return n; }
        using down =  idx< n-1 >;
        using up   =  idx< n+1 >;

        template< typename... T >
        using power_of  =  typename down::power_of< T... >::type >
    };


    template<>
    struct idx<0>
    {
        static constexpr auto eval() { return n; }
        using down =  idx< -1 >;
        using up   =  idx< +1 >;

        template< typename... T >
        using power_of  =  identity<T...>;
    };


    struct infinity
    {
    private:

        template<typename T, typename R = typename T::type>
        static R get_type_(T*, T*);


        template<typename T>
        static T get_type_(T*, ... );


        template<typename T>
        struct hull
        {
            using type  =  hull< decltype( get_type_<T>(nullptr, nullptr) ) >;
            using read  =  T;
        };


        template<typename>
        struct step;


        struct step_
        {
            template<typename X>
            using type_for  =  compute_power<typename X::type>;
        };


        struct terminate
        {
            template<typename X>
            using type_for
            {
                using type = typename T::read;
            };
        };


        template<typename T>
        struct compute_power
        {
            using choice  =  bool_<  id<T> == id<typename T::type>  >;

            using type =  typename conditional< choice, terminate, step_ >::
                          template type_for< X >::
                          type;
        }


    public:

        template<typename... T>
        using power_of  =  typename compute_power< hull< identity<T...> > >::type;
    };


    // power<N, F> = F^N
    template< typename... X >
    using power  =  typename subterms< X... >::
                    head::
                    template power_of< typename subterms< X... >::tail::template apply<identity> >;


    template< template<typename... > class F >
    struct monoid
    {

        template< typename... X >
        struct action
        {
            using type  =  action< F< X... > >;
            using read  =  identity< X... >;
        };

        template<typename... Idx>
        using power = typename aml::power< identity<Idx...>, action<X...>  >::read;
    };


    template<typename... T>
    struct hull
    {
        using type = identity<T...>;
    };


    template< typename... >
    struct delay;

    struct delay<>
    {
    private:

        template< template<typename...> Evaluate_ >
        using evaluate_subterm
        {
            template<typename... X>
            using apply_to  =  typename Evaluate_< identity<X...> >::type;
        };


        template<typename T>
        struct evaluate_
        {
        private:
            struct subevaluate_
            {
                using type =
                    typename term<T>::
                    subterms::
                    template pointwise_apply<evaluate_subterm<T, evaluate_> >::
                    template apply< typename term<T>::subterms::head::template apply_to >
            };

            struct remove_delay_
            {
                using terms = typename term<T>::subterms::tail;
                using type  = typename terms::tail::template apply< terms::head::template apply_to >;
            };

        public:

            using resolve_delay  =  bool_< term_id< typename term<T>::subterms::apply<delay> > == term_id< T > >;

            using type           =  typename conditional< remove_delay, remove_delay_, subevaluate_ >::type;
        };


        template<typename T, typename Depth = idx<1>, tyepname... no_args>
        struct evaluate_t
        {
            static_assert( sizeof...(no_args) == 0 );

            using type  =  typename conditional< bool_< Depth::eval() == 0 >,
                                                 T,
                                                 power<Depth, evaluate_< identity<T...> > > >;
        };

    public:

        template<typename... T>
        using evaluate = typename evaluate_t< T... >::type;
    };

    template<typename... X>
    using evaluate = typename delay<>::template evaluate< X... >;

}
