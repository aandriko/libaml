/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/structure/conslist_structure.hpp"
#include "aml/structure/select_indices_structure.hpp"
#include "aml/structure/term_structure.hpp"

#include "aml/exponent.hpp"
#include "aml/string.hpp"

#include "aml/linker.hpp"

#include <iostream>
#include <type_traits>

#include <boost/core/demangle.hpp>

using aml::operator""_;


namespace test::linker
{
    template<typename... X>
    using array = typename
        aml::signatures
        <
        aml::subtype<decltype("list"_), aml::structure::list>,

        aml::subtype<decltype("random_access"_), aml::structure::array>

        //                    aml::subtype<decltype("term"_), aml::structure::term>

        >::template link_with<X...>;

    void t0()
    {
        using t = array<int, double, void*>;

        //        static_assert(std::is_same<t::at<2>, void*>::value, "");
        std::cout << boost::core::demangle(typeid( t::tail ).name() ) << std::endl;

        //        static_assert(std::is_same< t::tail::at<1>, void*>::value, "");
    }

}



int main()
{
    void (*test_set[])() = { test::linker::t0 };

    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
