#pragma once

#include "./logic.hpp"


namespace aml
{
    void infinity() { }


    struct infinity_t
    {
        static constexpr auto eval() { return infinity; };
    };


    template<  template< typename... > class F
            ,  auto...  n
            >
    struct curry
    {
        static_assert( sizeof...(n) != 0 );
    };


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
    using currying_l = curry< F::template apply_to, N::eval()... >;


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


        template< typename X, auto >
        struct power_;


        template<typename X, auto n>
        struct power_intermediate_
        {
            using type = typename power_< typename X::type, n-1 >::type;
        };


        template<typename X>
        struct power_terminate_
        {
            using type  =  X;
        };


        template< typename X, auto n>
        struct power_
        {
            using type  =  typename bool_< n != 0 >::
                           template conditional<    power_intermediate_<X, n> ,
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

            template<typename>
            static constexpr void term_id() { };

            using continue_  =  bool_< term_id<decltype( next_step_<X>(nullptr, nullptr) )>  !=  term_id<X> >;
            using type  =  typename continue_::template conditional< seq_step<X>, seq_terminate<X> >::type;
        };

        template< void (*n)() >
        static constexpr bool is_finite_(decltype(nullptr), decltype(nullptr))
        {
            return false;
        }

        template< auto n >
        static constexpr bool is_finite_(decltype(nullptr), ... )
        {
            return true;
        }

        template<auto n>
        struct is_finite
        {
            static constexpr bool eval()
            {
                return is_finite_<n>(nullptr, nullptr);
            }
        };

    public:

        template<typename... X>
        using apply_to = typename is_one_parameter<X...>::type;

        template<typename X, auto n>
        using power  =  typename bool_< is_finite<n>::eval() >::
                        template conditional<  power_<X, n> ,  limit_<X> >::
                        type;


        template<typename... X>
        using limit  =  typename limit_< typename is_one_parameter<X...>::type >::type;
    };


    template<typename... X>
    using identity  =  typename composition<>::template apply_to< X... >;


    template< typename T, auto n = infinity>
    using power = typename composition<>::template power< T, n >;


    template< typename... T >
    using limit  =  typename composition<>::template limit<T...>;


    template< typename X, typename... N>
    using power_l  =  power< X, identity< N... >::eval()  >;


    template<  template< typename... > class...  >
    struct monoid;


    template< template<typename... > class F >
    struct monoid<F>
    {
        template< typename... X >
        struct action
        {
            using type     =  action< F< X... > >;

            struct subterms
            {
                template< template<typename... > class G >
                using apply =  G<X...>;
            };
        };


        template< auto        n
                , typename... X
                >
        using power  =  typename aml::power< action< X... >, n >::
                        subterms::
                        template apply< identity >;
    };


    template<>
    struct monoid<>
    {
        template< typename    F
                , typename    N
                , typename... X
                >
        using power_l  =  typename monoid< F::template apply_to >::
                          template power< N::eval(), X... >;
    };

}
