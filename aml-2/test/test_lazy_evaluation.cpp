#include "aml/lazy_evaluation.hpp"

#include <type_traits>

namespace test::lazy
{
    void test_hull()
    {
        struct x;
        struct y;

        static_assert( aml::hull<x>::id != aml::hull<y>::id );
        static_assert( std::is_same< aml::hull<x>::type, x >::value );
    }

    template< typename... >
    struct f {};

    template< typename X, typename... T >
    struct f<X, T... >
    {
        using type = X;
    };

    template<auto n>
    struct number
    {
        static constexpr auto eval() { return n; }
    };


    void test_evaluation()
    {
        using t0  =  aml::lazy_l< int >::type;

        using t1  =  aml::evaluate_l< int >;

        static_assert( std::is_same< t0, int >::value );
        static_assert( std::is_same< t1, int >::value );

        using t3  =  aml::lazy_l< aml::delay<2>, aml::curry<f>, int*,
                                  aml::lazy_l< aml::delay<1>, aml::curry<f>, int**, int*** > >;

        using t4  =  aml::lazy< f, 2 >::
                     apply_to<    int*,  aml::lazy<f>::apply_to<int**, int***>    >;

        static_assert( std::is_same< t3, t4 >::value );

        using t5 = aml::evaluate_l< t3, number<0> >;
        using t6 = aml::evaluate< t3, 0 >;

        static_assert( std::is_same<t5, t3>::value );
        static_assert( std::is_same<t5, t6>::value );

        using t7  =  aml::lazy_l< aml::delay<1>, aml::curry<f>, int*, f<int**, int*** > >;
        using t8  =  aml::lazy<f>::apply_to< int*, f< int**, int*** > >;

        static_assert( std::is_same< t7, t8 >::value );

        using t9   =  aml::evaluate_l< t3, number<1> >;
        using t10  =  aml::evaluate_l< t4, number<1> >;

        static_assert( std::is_same< t9, t10 >::value );

        static_assert( std::is_same< t7, t9 >::value );
        static_assert( std::is_same< t8, t10 >::value );

        using t11  =  f< int*, f<int**, int***> >;

        using t12  =  aml::evaluate_l< t3, number<2> >;
        using t13  =  aml::evaluate< t3, 2 >;

        static_assert( std::is_same< t12, t13 >::value );
        static_assert( std::is_same< t11, t12 >::value );

        using t14 = aml::evaluate_l< t3 >;
        using t15 = aml::evaluate< t3 >;

        static_assert( std::is_same< t14, t15 >::value );
        static_assert( std::is_same< t14, f< int*, f< int**, int*** > > >::value );
    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::lazy::test_hull,
        test::lazy::test_evaluation,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
