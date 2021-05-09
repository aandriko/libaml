// clang++ -std=c++20 -Wall -pedantic -I../include t0.cpp

#include <boost/core/demangle.hpp>
#include <iostream>

#include "aml/subtype/linker.hpp"
#include "aml/subtype/term.hpp"
#include "aml/subtype/list.hpp"


using aml::operator""_;

template<typename... x>
using term = aml::adt::signatures
             <
                 aml::adt::link<decltype("list"_), aml::adt::list>,
                 aml::adt::link<decltype("term"_), aml::adt::term>
             >::link_with<x...>;


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
    
    
    //    static_assert(template_is_equal<term< type >::template function, foo >::vlaue, "");
    //    static-assert(std::is_equal<


    //    std::cout << boost::core::demangle( typeid(t).name() ) << std::endl;
}
