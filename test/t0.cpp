// clang++ -std=c++20 -Wall -pedantic -I../include t0.cpp

#include <boost/core/demangle.hpp>
#include <iostream>

#include "aml/linker.hpp"
#include "aml/adt/term.hpp"
#include "aml/adt/list.hpp"


using aml::operator""_;

template<typename... x>
using term = aml::adt::signatures
             <
                 aml::subtype<decltype("list"_), aml::adt::list>,
                 aml::subtype<decltype("term"_), aml::adt::term>
             >::link_with<x...>;


template<typename... x>
using term2 = typename
    aml::adt::signatures
    <
       aml::subtype<::term<>, ::term >::ignore_linker
    >::
    template link_with<x...>::template subtype< ::term<> >;
    
template<typename... >
struct foo { };


template<template<typename...> class F>
struct Template { };

template<typename...>
struct Parameters { };

int main()
{
    using type = foo<foo<int, char, foo<> > >;
    using term_t = term<type>;

    
    std::cout << boost::core::demangle( typeid(term_t::function<>).name()  ) << std::endl;
    std::cout << boost::core::demangle( typeid(term_t::subterms::apply<Parameters>).name() ) << std::endl;


    static_assert(std::is_same< ::term2<foo<int> >, ::term<foo<int>> >::value, "" );
    
    //    static_assert(template_is_equal<term< type >::template function, foo >::vlaue, "");
    //    static-assert(std::is_equal<


    //    std::cout << boost::core::demangle( typeid(t).name() ) << std::endl;
}
