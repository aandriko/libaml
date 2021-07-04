/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/curry.hpp"
#include "aml/apply.hpp"
#include "aml/string.hpp"
#include "aml/basic_types.hpp"


#include <type_traits>


namespace test::curry
{
    template<typename...>
    struct F {};

    using curried_F = aml::curry<3, F>;

    void test_curry_and_uncurry()
    {
        using f = curried_F::apply_to<void*, void**, void***>::apply_to<int>;

        using g = aml::uncurry<3, curried_F::template apply_to>::template apply_to<void*, void**, void***, int>;

        using h = F<void*, void**, void***, int>;

        static_assert(std::is_same<f, h>::value, "");
        static_assert(std::is_same<g, h>::value, "");
    }
}


#include <iostream>
#include <string>


int main()
{

    void (*test_set[])() = { test::curry::test_curry_and_uncurry };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
