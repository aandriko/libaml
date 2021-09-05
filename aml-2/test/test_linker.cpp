/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


/*
#include "aml/adt/linker.hpp"


#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

#include <string>
#include <memory>

#include <cassert>
#include <vector>
#include <tuple>


namespace test::function_argument
{
    template<auto n> struct foo {};



    template<typename Linker, typename Value>
    struct Vector : public std::vector<Value>
    {
        Vector(std::initializer_list<double> l )
            :    std::vector<double>(l)
        { }


        Vector(Vector const& other)
            :    std::vector<double>(other)
        {
            std::cout << "copy constructing! " << std::endl;
        }


        Vector(Vector && other)
            :    std::vector<double>(std::move(other))
        {
            std::cout << "move constructing! " << std::endl;
        }

    };

    struct t1 { t1() = default; };
    struct t2 { t2() = default; };

    template<typename... X>
    using adt  =  aml::adt::link<  aml::adt::subtype<t1, Vector>
                                ,  aml::adt::subtype<t2, Vector > >::adt<double>;

    void test_dummy()
    {
        //        adt<double> x1;
        //        adt<double> x2( Vector<double>{1.1, -2.2 }, Vector<double>{-3.7});
        adt<double> x2( adt<double>::factor<t1>{1.1, -2.2 }, adt<double>::factor<t2>{-3.7});


        //        auto const& ww = x2.data_;
        //        std::vector<double> const& w = x2.data_.cref<t1>();
  
        //        for (auto const& el : w )
            //            std::cout << el << " : " ;
        //        std::cout << std::endl;

        //        std::cout << boost::core::demangle( typeid(adt<double>::data_t).name() ) << std::endl;

    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::function_argument::test_dummy,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
*/


int main() { } 
