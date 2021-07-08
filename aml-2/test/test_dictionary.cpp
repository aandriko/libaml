/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/dictionary.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>



namespace test::dictionary
{
    void test_empty_dictionary()
    {
        using d0 = aml::dictionary<>;

        static_assert( d0::size() == 0 );
        static_assert( std::is_same< d0::key_list,   aml::list<> >::value );
        static_assert( std::is_same< d0::value_list, aml::list<> >::value );
        static_assert( std::is_same< d0::entry_list, aml::list<> >::value );
        static_assert( std::is_same< d0::entry_set,  aml::set<> >::value );
        static_assert( std::is_same< d0::key_set,    aml::set<> >::value );

        using d0s  =  aml::dictionary<>::with_sentinel<char***>;
        static_assert( std::is_same< d0s::at<int>, char***>::value );

        using l0 = d0::map_to_values<>;
        static_assert( std::is_same< aml::list<>, l0 >::value );

        using l0s  =  d0s::map_to_values<>;
        static_assert( std::is_same< aml::list<>, l0s >::value );

        using l1  =  d0s::map_to_values<int, double, char***>;
        static_assert( std::is_same< aml::list<char***, char***, char***>, l1 >::value );

        using l2 = d0::add_entries<  aml::entry< int, double >
                                  ,  aml::entry< int, int* >
                                  ,  aml::entry< double, void >
                                  >;

        using l2_list = aml::list< aml::entry<int, int*>, aml::entry<double, void> >;
        static_assert( std::is_same< l2::entry_list, l2_list >::value );
    }


    struct sentinel_t;


    void test_type_with_sentinel()
    {
        using d1   =  aml::dictionary< aml::entry<int*, double > >;
        using d1_s  =  aml::dictionary< aml::sentinel<sentinel_t>, aml::entry<int*, double> >;

        static_assert( std::is_same< d1::with_sentinel<sentinel_t>, d1_s >::value );
    }

    template< typename Dictionary
            , typename Key
            , typename  = typename Dictionary::template at<Key>
            >
    aml::true_ at_is_successful_( std::nullptr_t, std::nullptr_t );


    template< typename, typename >
    aml::false_ at_is_successful_( std::nullptr_t, ... );


    template< typename Dictionary
            , typename Key >
    using at_is_successful  =  decltype( at_is_successful_<Dictionary, Key>(nullptr, nullptr));

    void test_at()
    {
        using d = aml::dictionary< aml::entry<int, double >,
                                   aml::entry<int*, double*>,
                                   aml::entry<int**, double**> >;


        using ds = d::with_sentinel<sentinel_t>;


        static_assert( at_is_successful< d,  int* >::eval() == true );
        static_assert( at_is_successful< ds, int* >::eval() == true );

        static_assert( at_is_successful< d, long >::eval() == false );
        static_assert( at_is_successful<ds, long >::eval() == true  );
    }

}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::dictionary::test_empty_dictionary
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
