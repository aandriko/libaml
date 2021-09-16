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
#include <tuple>


namespace test::adt
{
    template<typename Linker, typename... Value>
    struct Vector : public std::vector<Value...>
    {
        Vector() :    std::vector<Value...>() {}


        Vector(std::initializer_list<Value...> l )
        :    std::vector<Value...>(l)
        { }


        Vector(Vector const& other)
        :    std::vector<Value...>(other)
        {
            std::cout << "copy constructing! " << std::endl;
        }


        Vector(Vector && other)
        :    std::vector<Value...>(std::move(other))
        {
            std::cout << "move constructing! " << std::endl;
        }


        Vector& operator=(std::vector<Value...> const& other)
        {
            static_cast< std::vector<Value... >& >(*this) = static_cast< std::vector<Value...> const&>(other);
            return *this;
        }


        Vector& operator=(std::vector<Value...>&& other)
        {
            static_cast< std::vector<Value... >& >(*this) = static_cast< std::vector<Value...> &&>(other);
            return *this;
        }

    };


    struct t1 { t1() = default; };
    struct t2 { t2() = default; };

    template<typename... X>
    using vec_adt  =  typename aml::adt::signature<  aml::adt::subtype<t1, Vector>,  aml::adt::subtype<t2, Vector > >::
                      template adt<X...>;


    void test_constructor()
    {
        vec_adt<double> v;
        v.ref< t1 >() = std::vector<double>{-1.1, -2.2, -3.3 };

        auto show = []( auto const& v )
        {
            for (auto const& el : v)
            {
                std::cout << el << " : " ;
            }
            std::cout << std::endl << std::endl;
        };

        show( v.ref<t1>() );

        //        auto w(v);
    }

}

#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::adt::test_constructor
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}




