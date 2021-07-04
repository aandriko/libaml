/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/set.hpp"

#include "aml/curry.hpp"
#include "aml/apply.hpp"
#include "aml/string.hpp"


#include <type_traits>

namespace test::set
{
    using set_1 = aml::set<int, void, char>;

    void test()
    {
     static_assert( set_1::contains<int, char>::eval(), "");
     static_assert( set_1::contains<>::eval() == true, "");
     static_assert( set_1::contains<int, int*>::eval() == false, "");

    }
}


#include <iostream>
#include <string>


int main()
{

    void (*test_set[])() = { test::set::test };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
