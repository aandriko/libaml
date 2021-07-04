/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/term_algebra.hpp"


#include <type_traits>


namespace test::term_algebra
{
    template<typename...> class H { };


    void test_term()
    {
        static_assert( std::is_same<  aml::function<H>::apply_to<int>, H<int> >::value );

        static_assert( std::is_same< aml::function<H>::template apply_to<>, H<> >::value );

        static_assert( std::is_same<aml::term<int>::function<char*>, char*>::value );

        static_assert( std::is_same<aml::term<H<>>::function<>, H<> >::value );

        static_assert( std::is_same< H<int, int*>,

                                     aml::term< H<int, int*> >::
                                     subterms::
                                     apply< aml::term< H<int, int*> >::function >
                       >::value );

        static_assert( std::is_same< aml::term<H<int, int*> >::subterms, aml::subterms<int, int*> >::value );

        static_assert( std::is_same< aml::term< H<> >::subterms, aml::subterms<> >::value);
        static_assert( std::is_same< aml::subterms<>::template apply<H>, H<> >::value );

        static_assert( std::is_same< aml::subterms<int, int*>::template apply<H>,
                                     H<int, int*> >::value );

        static_assert( std::is_same< aml::subterms<int>::head, int >::value );

        static_assert( std::is_same< aml::subterms<int>::tail, aml::subterms<> >::value );

        static_assert( std::is_same< aml::subterms<int, int*>::head, int >::value );

        static_assert( std::is_same< aml::subterms<int, int*>::tail, aml::subterms<int*> >::value );
    }

    void test_is_same()
    {
        static_assert( aml::is_same<>::eval() );
        static_assert( aml::is_same<int*>::eval() );
        static_assert( aml::is_same<int*, int*>::eval() );
        static_assert( aml::is_same<int*, int*, int*>::eval() );
        static_assert( aml::is_same<int*, int*, int**>::eval() == false);
    }


}




#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::term_algebra::test_term,
        test::term_algebra::test_is_same
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
