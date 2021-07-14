/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/adt/linker.hpp"
#include "aml/string.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

#include <string>
#include <memory>

#include <cassert>
#include <vector>


namespace test::linker
{
    template<typename... X >
    using vec = std::vector< typename aml::list<X...>::head>;

    template<typename... X>
    using sptr = std::shared_ptr< typename aml::list<X...>::tail::head >;

    template<typename... X>
    using uptr = std::unique_ptr< typename aml::list<X...>::tail::head >;

    using aml::operator ""_;


    template<typename X, typename Y>
    using adt_2_t = typename aml::adt::link< aml::adt::subtype< decltype("vector"_), vec >
                                           , aml::adt::subtype< decltype("pointer"_), sptr >
                                           >::template adt<X, Y>;


    struct hello {};


    template<typename... T>
    struct keeper;

    template<typename T>
    struct keeper<T>
    {
        T data_;

        template<typename T_>
        explicit keeper(T_&& t) : data_(std::forward<T_>(t)) { }

        keeper(keeper const& other)
            : data_(other.data_)
        { }

        keeper(keeper && other)
            : data_(std::move(other.data_))
        { }
    };


    /*

//      This function does not compile: Indication of bug that cannot be found easily with std::vector
//      in test_dummy, where the compilation seemingly succeeds.

    void test_dummy_boom()
    {
        using adt = aml::adt::link< aml::adt::subtype<hello, keeper> >::adt<int>;
        adt x( aml::adt::type<hello>(-2) );
        std::cout << x[hello()].data_ << std::endl;
    }

    */


    void test_dummy()
    {
        using adt_1_t = aml::adt::link< aml::adt::subtype<hello, std::vector> >::adt<int>;

        adt_1_t adt_1;

        std::vector<int> const v_ {-11, -22, -33 };
        
        adt_2_t<int, double> adt_2( aml::adt::type<decltype("pointer"_)>( new double(4.3) )
                                  , aml::adt::type<decltype("vector"_)>(v_) );
                                    //                                    aml::adt::type<decltype("vector"_)>( std::vector<int>{-2, -4} ));


        std::cout << "vector: " << std::endl;
        auto const& v =  adt_2["vector"_];
        for (auto const& el : v)
        {
            std::cout << el << " : " << std::endl;
        }

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
