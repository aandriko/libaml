#include "aml/lazy_evaluation.hpp"

#include <type_traits>

namespace test::lazy
{
    template< typename... >
    struct f {};


    template< typename X, typename... T >
    struct f<X, T... >
    {
        using type = X;
    };


    void test_evaluation()
    {
        using aml::_;

        using t0  =  aml::lazy< int >::type;

        using t1  =  aml::evaluate< int >;

        static_assert( std::is_same< t0, int >::value );
        static_assert( std::is_same< t1, int >::value );

        using t3  =  aml::lazy< _<2>, aml::curry<f, _<0> >::apply_to<>, int*,
                                aml::lazy< _<1>, aml::curry<f, _<0> >::apply_to<>, int**, int*** > >;


        using t5 = aml::evaluate< t3, _<0> >;

        static_assert( std::is_same<t5, t3>::value );


        using t7  =  aml::lazy< _<1>, aml::curry<f, _<0> >::apply_to<>, int*, f<int**, int*** > >;


        using t9   =  aml::evaluate< t3, _<1> >;


        static_assert( std::is_same< t7, t9 >::value );


        using t11  =  f< int*, f<int**, int***> >;

        using t12  =  aml::evaluate< t3, _<2> >;

        static_assert( std::is_same< t11, t12 >::value );

        using t14 = aml::evaluate< t3 >;

        static_assert( std::is_same< t14, f< int*, f< int**, int*** > > >::value );

    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::lazy::test_evaluation,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;
}
