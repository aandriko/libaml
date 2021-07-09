/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/adt/record.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

#include <string>


namespace test::record
{
    struct hello;
    struct world;

    void test_0()
    {
        aml::adt::record< aml::entry<hello, double*>, aml::entry<world, std::string> > r( nullptr, "hello");

        std::cout << r.cref<world>() << std::endl;
    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::record::test_0,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
