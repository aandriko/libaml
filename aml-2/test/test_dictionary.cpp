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
        static_assert( std::is_same< d0s::lookup<int>, char***>::value );

        using l0 = d0::translate_keys<>;
        static_assert( std::is_same< aml::list<>, l0 >::value );

        using l0s  =  d0s::translate_keys<>;
        static_assert( std::is_same< aml::list<>, l0s >::value );

        using l1  =  d0s::translate_keys<int, double, char***>;
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
            , typename  = typename Dictionary::template lookup<Key>
            >
    aml::true_ lookup_is_successful_( std::nullptr_t, std::nullptr_t );


    template< typename, typename >
    aml::false_ lookup_is_successful_( std::nullptr_t, ... );


    template< typename Dictionary
            , typename Key >
    using lookup_is_successful  =  decltype( lookup_is_successful_<Dictionary, Key>(nullptr, nullptr));

    void test_lookup()
    {
        using d = aml::dictionary< aml::entry<int, double >,
                                   aml::entry<int*, double*>,
                                   aml::entry<int**, double**> >;


        using ds = d::with_sentinel<sentinel_t>;


        static_assert( lookup_is_successful< d,  int* >::eval() == true );
        static_assert( lookup_is_successful< ds, int* >::eval() == true );

        static_assert( lookup_is_successful< d, long >::eval() == false );
        static_assert( lookup_is_successful<ds, long >::eval() == true  );
    }

    template<typename T>
    using is_ptr = aml::bool_<std::is_pointer<T>::value>;

    void test_add_and_remove()
    {
        using d = aml::dictionary<  aml::entry<double, int>
                                 ,  aml::entry< double*, int*>
                                 ,  aml::entry< double** ,int**>
                                 ,  aml::entry< void,  double >  >;

        using d2 = d::key_list::partition_with<is_ptr>::accepted::template apply< d::remove_keys >;

        static_assert( std::is_same< d2, aml::dictionary< aml::sentinel<>,
                                                          aml::entry<double, int>,
                                                          aml::entry<void, double>>>::value );


        using e  =  d::add_entries< aml::entry< void, void>, aml::entry< char*, int > >;
        using f  =  d::add_entries< aml::entry< char*, int>, aml::entry< void, void > >;
        using g  =  d::add_entries< aml::entry<void*, void*> >;

        using h  =  d::remove_keys< aml::entry<void*, void*> >;

        using e_list  =  aml::list< aml::entry< double,   int>
                                  , aml::entry< double*,  int*>
                                  , aml::entry< double**, int**>
                                  , aml::entry< void,     void>
                                  , aml::entry< char*,    int> >;


        using f_list  =  aml::list< aml::entry<double, int>
                                  , aml::entry<double*, int*>
                                  , aml::entry< double**, int**>
                                  , aml::entry< void, void>
                                  , aml::entry< char*, int> >;


        using g_list  =  aml::list< aml::entry< double,   int >
                                  , aml::entry< double*,  int* >
                                  , aml::entry< double**, int** >
                                  , aml::entry< void,     double >
                                  , aml::entry< void*,    void* > >;


        using h_list  =  aml::list< aml::entry< double, int >
                                  , aml::entry< double*, int* >
                                  , aml::entry< double**, int** >
                                  , aml::entry< void, double > >;


        static_assert( std::is_same< e::entry_list, e_list >::value );
        static_assert( std::is_same< f::entry_list, f_list >::value );
        static_assert( std::is_same< g::entry_list, g_list >::value );
        static_assert( std::is_same< h::entry_list, h_list >::value );
    }

    template<  typename Dictionary
            ,  typename Key_List
            ,  typename  =  typename Key_List::
                            template apply< Dictionary::template translate_keys >
            >
    aml::true_ translation_exists_(std::nullptr_t, std::nullptr_t);


    template<  typename,  typename  >
    aml::false_ translation_exists_(std::nullptr_t, ... );


    template<  typename Dictionary
            ,  typename... Key
            >
    using translation_exists  =  decltype(
               translation_exists_<Dictionary, aml::list<Key...> >(nullptr, nullptr)
                                      );

    void test_translation()
    {
        using d = aml::dictionary<  aml::entry<double, int>
                                 ,  aml::entry< double*, int*>
                                 ,  aml::entry< double** ,int**>
                                 ,  aml::entry< void,  double >  >;

        using d_with_sentinel = d::with_sentinel<void***>;

        static_assert( translation_exists<d, double, double const, void>::eval() == false );
        static_assert( translation_exists<d_with_sentinel, double, double const, void>::eval() );

        using values_1    =  d_with_sentinel::translate_keys< void, double const, double >;
        using expected_1  =  aml::list< double, void***, int >;

        static_assert( std::is_same< values_1, expected_1 >::value );

        using value_2    =  d::translate_keys<double*, double**>;
        using expected_2 =  aml::list<int*, int**>;

        static_assert( std::is_same< value_2, expected_2 >::value );
    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::dictionary::test_empty_dictionary,
        test::dictionary::test_type_with_sentinel,
        test::dictionary::test_lookup,
        test::dictionary::test_add_and_remove,
        test::dictionary::test_translation
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
