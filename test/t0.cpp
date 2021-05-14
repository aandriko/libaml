// clang++ -std=c++20 -Wall -pedantic -I../include t0.cpp


#include "aml/sort.hpp"
#include "aml/partition_and_join.hpp"
#include <boost/core/demangle.hpp>
#include <iostream>

#include "aml/select_indices.hpp"
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




template<typename X, typename ConsList>
using make_right_indexed = typename ConsList::template cons< numbered_item< ConsList::size(), X> > ;

template<typename ConsList, typename X>
using make_left_indexed = make_right_indexed<X, ConsList>;


using numbered_right = aml::conslist<int, char, double>::rfold_with<make_right_indexed, aml::conslist<> >;
using numbered_left = aml::conslist<int, char, double>::lfold_with<make_left_indexed, aml::conslist<> >;
using numbered_nicely = aml::conslist<int, char, double>::reverse::lfold_with<make_left_indexed, aml::conslist<> >::reverse;


template<typename T>
struct pred
{
    static constexpr bool eval() { return sizeof(T) == 1; }
};

using partition_t = aml::partition<char, int, void*, unsigned char, std::nullptr_t>::with<pred>;

using t_join = aml::join< partition_t::accepted, partition_t::rejected, aml::conslist<void, void> >;    


template<typename X, typename Y>
struct less
{
    static constexpr bool eval() { return sizeof(X) < sizeof(Y); }
};

using tlist = aml::conslist<double, char, float, unsigned char>;
//using tlist = aml::conslist<char, double>::reverse;

using slist = tlist::template apply< aml::sort >::template with<less>;

int main()
{
    std::cout << boost::core::demangle( typeid(tlist***).name() ) << std::endl;
    std::cout << boost::core::demangle( typeid(slist***).name() ) << std::endl << std::endl;
    
        
    
    std::cout << "partition accept: " << boost::core::demangle( typeid(partition_t::accepted* ).name() ) << std::endl
              << "partition reject: " << boost::core::demangle( typeid(partition_t::rejected* ).name() ) << std::endl << std::endl;
    
    
    using type = foo<foo<int, char, foo<> > >;
    using term_t = term<type>;

    
    std::cout << boost::core::demangle( typeid(term_t::function<>).name()  ) << std::endl;
    std::cout << boost::core::demangle( typeid(term_t::subterms::apply<Parameters>).name() ) << std::endl;
    //    std::cout << "t_join: " << boost::core::demangle( typeid(t_join).name() ) << std::endl;

    static_assert(std::is_same< ::term2<foo<int> >, ::term<foo<int>> >::value, "" );
    
    using t0 = aml::conslist<int, void, char>;

    using t1 = t0::tail::tail::head;

    static_assert(std::is_same<t1, char>::value, "");


    using t2 = aml::select_indices<2, 4, 2>::from<int, char, double, void, char*, void*>;

    std::cout << boost::core::demangle( typeid(t2).name() )  << std::endl;


    //    using t3 = numbered;

    
    std::cout << "          left fold:  " << boost::core::demangle(typeid(numbered_left*).name()) << std::endl;
    std::cout << "          right fold: " << boost::core::demangle(typeid(numbered_right*).name()) << std::endl;
    std::cout << "nicely numbered fold: " << boost::core::demangle(typeid(numbered_nicely*).name()) << std::endl;
    
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
