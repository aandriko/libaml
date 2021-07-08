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
