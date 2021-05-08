#include "adt/linker.hpp"
/*
#include "adt/term.hpp"
#include "syntax.hpp"


namespace aml::sig
{
    template<typename Link, typename... Subterms>
    struct subterms
    {
    public:
        template<template<typename...> class Function>
        using apply = Function<Subterms...>;

        
        template<template<typename...> class Function>
        using pointwise_apply = 
            typename syntax::subterms< Function<Subterms>... >::template apply< Link::template abstract_type  >;

    };
      
   

    template<typename Link, typename... x>
    struct modify_front_and_back
    {
        template<typename... y>
        using push_front = typename Link::template abstract_type<y..., x...>;

        template<typename... y>
        using push_back = typename Link::template abstract_type<x..., y...>;
    };


    template<typename Link, typename... x>
    struct cons_list;

    
    template<typename Link>
    struct cons_list<Link>
    {
        using tail = typename Link::template abstract_type<>;
    };


    template<typename Link, typename h, typename... t>
    struct cons_list<Link, h, t...>
    {
        using head = h;
        using tail = typename Link::template abstract_type<t...>;
    };
}





template<typename... x>
using list = typename
    aml::adt::subtypes
    <
       aml::sig::subterms,
       aml::sig::modify_front_and_back,
       aml::sig::cons_list
    >::template link_with_parameters<x...>;




template<typename x>
using term = typename
    aml::adt::subtypes
    <
    aml::adt::term_t,
    aml::adt::trivially_linked< ::list>::template t
    >::template link_with_parameters<>::template subtype<   aml::adt::term_t  >::template with_parameters<x>;

*/
int main()
{
    //  list<char, int>::push_back<void>::head s;
    //    term<int>::function<void> t;
    //    term<int> t;
}
