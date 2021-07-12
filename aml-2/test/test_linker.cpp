/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/adt/linker.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

#include <string>
#include <memory>

#include <cassert>
#include <vector>

namespace test::linker
{
    struct hello {};
    void test_dummy()
    {
        aml::adt::link< aml::adt::subtype<hello, std::vector> >::adt<int> adt_1;
        aml::adt::link< aml::adt::subtype<hello, std::vector> >::adt<int> adt_2( aml::adt::sub<hello>( 4 ) );

        int x;
        static_cast<void>(x); // breakpoint for gdb
    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::linker::test_dummy,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
