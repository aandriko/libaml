#include "aml/functional.hpp"

#include <type_traits>


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

    template< auto n >
    struct number { static constexpr auto eval() { return n; } };

    void test_curry()
    {
        using c0  =  aml::curry<F, 2>::apply_to<t<1>, t<2> >::
                                       apply_to<t<3>, t<4>, t<5>, t<6> >;

        static_assert( std::is_same< c0, f >::value );

        using c1  =  aml::curry< F, 1, 3, 2 >::
            apply_to<  t<1>  >::
            template apply_to<  t<2>, t<3>, t<4>  >::
            template apply_to<  t<5>, t<6>  >::
            template apply_to<>;

        static_assert( std::is_same< c1, f >::value );

        using c2  =  aml::curry<F, 3, 1>::
                     apply_to< t<1>, t<2>, t<3> >::
                     apply_to< t<4> >::
                     apply_to< t<5>, t<6> >;

        static_assert( std::is_same< c2, f >::value );

        using c3  =  aml::curry<F>::apply_to< t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >;

        static_assert( std::is_same< c3, f >::value );


        using cb1  =  aml::curry_and_bind<F, t<1>, t<2> >::
                      apply_to<t<3>, t<4>, t<5>, t<6> >;

        using cb2  =  aml::curry_and_bind<F>::
                      apply_to< t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >;

        using cb3  =  aml::curry_and_bind<F, t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >::
                      apply_to<>;


        static_assert( std::is_same< cb1, f >::value );
        static_assert( std::is_same< cb2, f >::value );
        static_assert( std::is_same< cb3, f >::value );


        using mc1  =  aml::make_currying< aml::curry<F> >::apply_to< t<1>, t<2>, t<3>, t<4>, t<5>, t<6> >;

        using mc2  =  aml::make_currying< aml::curry<F>, number<2> >::
                      apply_to< t<1>, t<2> >::
                      apply_to< t<3>, t<4>, t<5>, t<6> >;

        using mc3  =  aml::make_currying< aml::curry<F>, number<2>, number<3> >::
                      apply_to< t<1>, t<2> >::
                      apply_to< t<3>, t<4>, t<5> >::
                      apply_to< t<6> >;

        static_assert( std::is_same< mc1, f >::value );
        static_assert( std::is_same< mc2, f >::value );
        static_assert( std::is_same< mc3, f >::value );
    }
}




#include <iostream>
#include <string>



int main()
{
    void (*test_set[])() =
    {
        test::functional::test_curry,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
