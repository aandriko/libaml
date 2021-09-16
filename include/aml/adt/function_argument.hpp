/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////

//#pragma once

#include "../object.hpp"
#include "../logic.hpp"


namespace aml::adt
{
    template< typename Number >
    struct function_argument;

    template<>
    struct function_argument< num<1> >
    {
        template<  typename    Head
                ,  typename... Tail
                >
        static consteval Head&& from_header( Head&& h, Tail&&...t )
        {
            return static_cast<Head&&>(h);
        }
    };

    template<typename Number>
    struct function_argument
    {
        template<  typename    Head
                ,  typename... Tail
                >
        static consteval decltype(auto) from_header( Head&& h, Tail&&... t )
        {
            return function_argument< typename Number::down >::from_header( static_cast<Tail&&>(t)... );
        }
    };

    template<typename N, typename... Args>
    consteval decltype(auto) get_argument(Args&&... args)
    {
        return function_argument<N>::from_header( static_cast<Args&&>(args)... );
    };
}


template<auto n> struct foo {};

#include <iostream>
#include <boost/core/demangle.hpp>
#include <vector>


template<typename T>
struct keeper
{
    template<typename S>
    keeper(S&& t) : t_(std::forward<S>(t) )
    { }

    T t_;
};

template<typename Value>
struct Vector : public std::vector<Value>
{
    Vector(std::initializer_list<double> l )
    : std::vector<double>(l)
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



int main()
{
    foo<  aml::adt::get_argument<aml::num<3> >( 1, 2, 3 ) > f;

    std::cout << boost::core::demangle( typeid(f).name() ) << std::endl;

    keeper< Vector<double> > k1( aml::adt::get_argument<aml::num<2> >( Vector<double>{1, 1.11, 2.22 },
                                                                            Vector<double>{-2, -3.3, -4.44 } ));;

    std::cout << boost::core::demangle( typeid(k1).name() ) << std::endl;
    auto const& v = k1.t_;

    for (auto const& el : v )
        std::cout << el << " : ";
    std::cout << std::endl;
}
