#include "aml/object.hpp"

#include <type_traits>

namespace aml
{
    template< typename... X>
    struct list
    {
        template<template<typename...> class F>
        using apply  =  F<X...>;
    };
}


namespace test::object
{
    int f(double) { return 2; }

    template<auto... > struct c { } ;

    void test_object()
    {
        static_assert( aml::object<f>::eval() == f );

        using l   =  aml::object<>::list<f, 3, nullptr>;
        using lt  =  l::as_types;
        using l_  =  lt::template apply<aml::object<>::template from_types>;

        static_assert( std::is_same<l, l_>::value );
    }

    void test_number()
    {
        static_assert( std::is_same< aml::num<aml::infinity>,
                       aml::num<aml::infinity>::down>::value );

        static_assert( std::is_same< aml::num<aml::infinity>,
                       aml::num<aml::infinity>::up>::value );

        static_assert( std::is_same<aml::infinity_t,
                       aml::num<aml::infinity> >::value );

        static_assert( aml::infinity_t::eval() == aml::infinity );

        static_assert( aml::num<3>::eval() == 3 );

        static_assert( std::is_same< aml::num<0>::down, aml::num<-1> >::value );

        static_assert( std::is_same< aml::num<0>::up, aml::num<+1> >::value );
    }

}




#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::object::test_object,
        test::object::test_number
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
