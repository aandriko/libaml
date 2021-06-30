#include "aml/functional.hpp"

#include <type_traits>


#include <iostream>
#include <boost/core/demangle.hpp>

namespace test::functional
{
    template< typename T1
            , typename T2
            , typename T3
            , typename T4
            , typename T5
            , typename T6
            , typename...
            >
    struct F
    {
        using type = F<T1, T2, T3, T4, T5, T6>;
    };


    template< int n >
    struct t;


    using f  =  F< t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >;


    template<template<typename...> class F>
    struct bound_function_
    {
        template< typename... X >
        using apply_to = F< X... >;
    };


    //    template< auto n >
    //  struct number { static constexpr auto eval() { return n; } };

    void test_curry()
    {
        using aml::_;

        using c0  =  aml::curry<F, _<2> >::apply_to<t<1>, t<2> >::apply_to<t<3>, t<4>, t<5>, t<6> >;

        static_assert( std::is_same< c0, f >::value );

        using c1  =  aml::curry< F, _<1>, _<3>, _<2> >::
            apply_to<  t<1>  >::
            template apply_to<  t<2>, t<3>, t<4>  >::
            template apply_to<  t<5>, t<6>  >::
            template apply_to<>;

        static_assert( std::is_same< c1, f >::value );

        using c2  =  aml::curry<F, _<3>, _<1> >::
                     apply_to< t<1>, t<2>, t<3> >::
                     apply_to< t<4> >::
                     apply_to< t<5>, t<6> >;

        static_assert( std::is_same< c2, f >::value );

        using c3  =  aml::curry<F, aml::infinity>::apply_to< t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >::apply_to<>;

        static_assert( std::is_same< c3, f >::value );


        using cb1  =  aml::curry_and_bind<F, t<1>, t<2> >::apply_to<t<3>, t<4>, t<5>, t<6> >;

        using cb2  =  aml::curry_and_bind<F>::apply_to< t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >;

        using cb3  =  aml::curry_and_bind<F, t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >::apply_to<>;


        static_assert( std::is_same< cb1, f >::value );
        static_assert( std::is_same< cb2, f >::value );
        static_assert( std::is_same< cb3, f >::value );
    }


    template<typename...> class K_ {};
    template<typename >   class H_ {};
    template<typename...> class G_ {};


    void test_composition()
    {
        using aml::_;

        using t1  =  K_<H_<G_<int, void, char> > >;
        using c1  =  aml::composition<K_, H_, G_>::apply_to<int, void, char>;
        using c2  =  aml::composition< aml::composition<K_, H_>::apply_to, G_>::apply_to<int, void, char>;
        using c3  =  aml::composition< K_,
                                       aml::composition<H_, G_>::template apply_to >::
                     apply_to<int, void, char>;

        static_assert( std::is_same< t1, c1 >::value );
        static_assert( std::is_same< t1, c2 >::value );
        static_assert( std::is_same< t1, c3 >::value );

        static_assert( std::is_same< aml::composition<>::apply_to<int*>,
                       int* >::value );

        static_assert( std::is_same< aml::identity<int*>, int* >::value );

        struct T0
        {
            struct type_
            {
                struct type;
            };

            using type = type_;
        };


        using T0_ = aml::composition<>::power<T0, _<0> >;

        static_assert( std::is_same< T0_, T0 >::value );
        static_assert( std::is_same<T0, aml::power<T0, _<0> >>::value );


        using T0_t  =  aml::composition<>::power< T0, _<1> >;


        static_assert( std::is_same< T0_t, typename T0::type >::value );
        static_assert( std::is_same< T0_t, aml::power<T0, _<1> > >::value );


        using T0_tt  =  aml::composition<>::power<T0, _<2> >;


        static_assert( std::is_same< T0_tt, T0::type::type >::value );
        static_assert( std::is_same< T0_tt, aml::power<T0, _<2> > >::value );


        struct n0 { static constexpr auto eval() {  return 0; } };
        struct n1 { static constexpr auto eval() {  return 1; } };
        struct n2 { static constexpr auto eval() {  return 2; } };
    }

    void test_limit()
    {
        struct T0
        {
            struct type_
            {
                struct type;
            };

            using type = type_;
        };


        using limit_t   =  aml::composition<>::limit<T0>;
        using limit_t_  =  aml::limit<T0>;


        static_assert( std::is_same< limit_t, T0::type::type >::value );
        static_assert( std::is_same< limit_t, limit_t_ >::value );


        struct has_type_forever
        {
            struct type_
            {
                using type = type_;
            };

            using type = type_;
        };

        // has_type_forever::type::type::type .... == has_type_foever::type;


        using limit_1  =  aml::composition<>::limit< has_type_forever >;
        using limit_2  =  aml::limit< has_type_forever >;

        static_assert( std::is_same<limit_1, limit_2>::value );
        static_assert( std::is_same<limit_1, has_type_forever::type >::value );

        using power_1  =  aml::composition<>::power< has_type_forever, aml::infinity >;
        using power_2  =  aml::power< has_type_forever, aml::infinity >;

        static_assert( std::is_same<limit_1, power_1>::value );
        static_assert( std::is_same<limit_2, power_2>::value );
    }


    template<typename... >
    struct X;

    template< typename A, typename B >
    struct X< A, B >;

    template< typename A, typename B, typename C, typename... D >
    struct X< A, B, C, D... >
    {
        using type  =  X<A, B, D...>;
    };


    template<  template< typename... > class F  >
    struct function_
    {
        template< typename... X >
        using apply_to = F< X... >;
    };


    void test_monoid()
    {
        using aml::_;

        using t0  =  aml::monoid<X>::power<_<0> >::apply_to< int** >;
        using t1  =  aml::monoid<X>::power< _<1> >::apply_to<int*, int**, int***, int****>;
        using t2  =  aml::monoid<X>::power< _<2> >::apply_to<int*, int**, int***, int****>;


        static_assert( std::is_same< t0, int** >::value );
        static_assert( std::is_same< t1, X< int*, int**, int***, int**** > >::value );
        static_assert( std::is_same< t2, X< X< int*, int**, int***, int**** > > >::value );
    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::functional::test_curry,
        test::functional::test_composition,
        test::functional::test_limit,
        test::functional::test_monoid,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
