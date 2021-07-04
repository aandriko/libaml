#include "aml/set.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

namespace aml
{
    template< typename... X>
    struct list
    {
        template<template<typename...> class F>
        using apply  =  F<X...>;
    };
}


namespace test::set
{
    using aml::_;

    using s0   =  aml::set<>;
    using s1   =  aml::set< _<2> >;
    using s2a  =  aml::set< _<1>, _<2> >;
    using s2b  =  aml::set<_<2>, _<1> >;
    using s3   =  aml::set< _<1>, _<3>, _<2> >;
    using s5   =  aml::set< _<1>, _<2>, _<3>, _<4>, _<5> >;


    void test_empty_set()
    {
        static_assert( s0() == s0() );
        static_assert( s0()  <= s0() );
        static_assert( s0() >= s0() );
        static_assert( ! (s0() != s0()) );
        static_assert( ! (s0() < s0())  );
        static_assert( ! (s0() > s0() ) );

        static_assert( std::is_same< s0::add_elements<>, s0 >::value );
        static_assert( std::is_same< s0::add_elements< _<2>, _<3> >, aml::set< _<2>, _<3> > >::value );
    }


    void test_size()
    {
        static_assert( s0::size() == 0 );
        static_assert( s1::size() == 1 );
        static_assert( s2a::size() == 2 );
        static_assert( s3::size() == 3 );
        static_assert( s5::size() == 5 );
    }


    void test_add_elements()
    {
        static_assert( std::is_same<s0,  s0::add_elements<> >::value );
        static_assert( std::is_same<s1,  s0::add_elements<_<2> > >::value );
        static_assert( std::is_same<s2a, s0::add_elements< _<1>, _<2> > >::value );
        static_assert( std::is_same<s5,  aml::set< _<1>, _<2> >::add_elements< _<3> >::add_elements< _<4>, _<5> > >::value );

        static_assert( std::is_same< aml::set<_<2> >::add_elements< _<2>, _<2> >, aml::set<_<2> > >::value );
    }


    void test_remove_elements()
    {
        static_assert( std::is_same< s0::remove_elements<>, s0 >::value );
        static_assert( std::is_same< s1::remove_elements< _<2>, _<3>, _<2> >, s0 >::value );
        static_assert( std::is_same< s3::remove_elements< _<2>, _<1>, _<4> >, aml::set< _<3> > >::value );
    }


    void test_contains()
    {
        static_assert( s0::contains<>::eval() );
        static_assert( s0::contains<_<1> >::eval() == false );
        static_assert( s3::contains< _<2>, _<1> >::eval() );
        static_assert( s3::contains_any_of< _<2>, _<8> >::eval() );
        static_assert( s3::contains_any_of< _<8>, _<2> >::eval() );
        static_assert( s3::contains_none_of< _<8>, _<5>, _<-1> >::eval() );
        static_assert( s3::contains_none_of< _<1>, _<8> >::eval() == false );
    }

    template<typename X>
    using is_even = aml::bool_< X::eval() % 2 == 0 >;

    void test_subset_by_predicate()
    {
        using p0  =  s0::subset_by_predicate< is_even >;
        using p1  =  s1::subset_by_predicate< is_even >;
        using p2a =  s2a::subset_by_predicate< is_even >;
        using p2b =  s2b::subset_by_predicate< is_even >;
        using p3  =  s3::subset_by_predicate< is_even >;
        using p5  =  s5::subset_by_predicate< is_even >;

        static_assert( std::is_same < p0, aml::set<> >::value );
        static_assert( std::is_same < p1, aml::set< _<2> > >::value );
        static_assert( std::is_same < p2a, aml::set<_<2> > >::value );
        static_assert( std::is_same < p2b, aml::set< _<2> > >::value );
        static_assert( std::is_same < p3, aml::set< _<2> > >::value );
        static_assert( std::is_same < p5, aml::set< _<2>, _<4> > >::value );
    }

    void test_set_operations_and_relations()
    {
        using s1  =  aml::set< _<1>, _<2>, _<3>             >;
        using s2  =  aml::set<       _<2>, _<3>, _<4>       >;
        using s3  =  aml::set<             _<3>, _<4>, _<5> >;
        using s4  =  aml::set< _<1>, _<2>, _<3>, _<4>       >;

        using s2a = aml::set< _<3>, _<2>, _<4> >;

        static_assert( std::is_same< decltype(s1() & s2() & s3()), aml::set<_<3>> >::value );

        using s_13 = decltype(s1() | s3());

        static_assert( std::is_same< s_13, aml::set< _<1>, _<2>, _<3>, _<4>, _<5> > >::value );

        static_assert( std::is_same< decltype( s1() - s2() ), aml::set< _<1> > >::value );
        static_assert( std::is_same< decltype( aml::set<>() - aml::set<>() ), aml::set<> >::value );

        static_assert(  s2a() == s2()  );
        static_assert(   s1() < s4()   );
        static_assert(   s4() > s1()   );
        static_assert(   s1()  <= s4() );
        static_assert(   s4() >= s1()  );
        static_assert(   s3() != s4()  );

        static_assert( !( s3() < s3() ) );
        static_assert( !( s3() > s3() ) );
        static_assert( s3() <= s3() );
        static_assert( s3() >= s3() );
        static_assert( s3() == s3() );
        static_assert( ! (s3() != s3() ) );
    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::set::test_empty_set,
        test::set::test_size,
        test::set::test_add_elements,
        test::set::test_remove_elements,
        test::set::test_contains,
        test::set::test_subset_by_predicate,
        test::set::test_set_operations_and_relations,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
