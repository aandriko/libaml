// clang++ -std=c++20 -Wall -pedantic -I../include t0.cpp

#include <boost/core/demangle.hpp>
#include <iostream>

#include "aml/select.hpp"
#include "aml/linker.hpp"
#include "aml/structure/term.hpp"
#include "aml/structure/list.hpp"


using aml::operator""_;

template<typename... x>
using term = aml::signatures
             <
                 aml::subtype<decltype("list"_), aml::structure::list>,
                 aml::subtype<decltype("term"_), aml::structure::term>
             >::link_with<x...>;


template<typename... x>
using term2 = typename
    aml::signatures
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

template<int n>
struct tt
{
    using type = tt<n+1>;
};

template<typename...> class F { };


template<int n, typename X>
struct numbered_item;

template<typename... T>
struct make_indexed_;

template<typename T, typename ConsList>
struct make_indexed_<T, ConsList>
{
    using type = 
        typename ConsList::template cons< numbered_item< ConsList::size(), T> > ;
};

/*
struct fold_stop;


template<typename T>
struct make_indexed_<T, fold_stop>
{
    using type = aml::conslist< numbered_item<0, T> >;
};
*/

template<typename... T>
using make_indexed = typename make_indexed_<T...>::type;


//using delme = make_indexed<>

//using numbered = aml::conslist<int, char, double>::fold_with<make_indexed, aml::conslist<> >::on_the_right;
    ;

int main()
{
    using type = foo<foo<int, char, foo<> > >;
    using term_t = term<type>;

    
    std::cout << boost::core::demangle( typeid(term_t::function<>).name()  ) << std::endl;
    std::cout << boost::core::demangle( typeid(term_t::subterms::apply<Parameters>).name() ) << std::endl;


    static_assert(std::is_same< ::term2<foo<int> >, ::term<foo<int>> >::value, "" );
    
    using t0 = aml::conslist<int, void, char>;

    using t1 = t0::tail::tail::head;

    static_assert(std::is_same<t1, char>::value, "");

    //    using t2 = aml::head<int, void, char>;

    //    using t3 = aml::identity<int>;

    //    t2 zzzz = 5;
    //static_assert(std::is_same<t2, int>::value, "");

    using t3 = aml::select<2, 4, 2>::from<int, char, double, void, char*, void*>::with_collector<aml::conslist>;

    //    std::cout << boost::core::demangle( typeid(t3*).name() )  << std::endl;

    //    std::cout << "fold: " << boost::core::demangle(typeid(numbered*).name()) << std::endl;
    
    //    using t3 = aml::function::power< aml::exp<0>, F >::apply_to<char**>;

    //    static_assert(std::is_same<char**, t3>::value, "");
    
        

    /*
    using t3 = aml::tail< aml::tail< aml::conslist<tt<0>, tt<1>, tt<2> > > >::head;


    using t4 = aml::function::power< aml::exp<0>, aml::tail >::apply_to< aml::conslist<tt<0>, tt<1>, tt<2> > >;
    using t44 = aml::function::power< aml::exp<0>, aml::tail >::apply_to< aml::conslist< tt<0> > >;

    

    
   
    std::cout << "t4: " << boost::core::demangle( typeid(t4*).name() )  << std::endl;
    std::cout << "t44: " << boost::core::demangle( typeid(t44*).name() )  << std::endl;    

    
    using t5 = aml::power<aml::exp<1>, tt<0>>;
    */
    //    using t5 = aml::function::power< aml::exp<2>, F>::template apply_to<int, char>;

    //    std::cout << boost::core::demangle(typeid(t5).name()) << std::endl;
    //    std::cout << boost::core::demangle(typeid(t5).name()) << std::endl;

    //    static_assert( std::is_same<aml::head<int, void>, int>::value, "") ;

    //    static_assert( std::is_same< aml::tail<void, int>::template apply< aml::identity >, int >::value, "");


    //    using t6 = aml::function::power<aml::exp<2>, aml::tail>::template apply_to<void, tt<0>, tt<1>, tt<2>, tt<3> >; //::template apply<aml::head>;


}
