#pragma once

#include <type_traits>
#include <utility>

#include "../parameters.hpp" 


namespace aml::adt
{    
    template<typename Symbol, template<typename...> class SubType>
    struct link;
}


namespace aml::adt::dtl::linker
{
    template<typename T>
    struct extract_subtype_template_from;

    
    template<typename Symbol, template<typename...> class SubType>
    struct extract_subtype_template_from<link<Symbol, SubType> >
    {
        template<typename... x>
        using apply_to = SubType<x...>;
    };

    
    template<typename... lookup_failure>
    struct resolve_symbol
    {
        struct type
        {
            static_assert( (sizeof...(lookup_failure) > 1), "symbol could not be found");
        };
    };

    
    template<typename symbol,
             typename    x_h, template<typename...> class    y_h,
             typename... x_t, template<typename...> class... y_t>
    struct resolve_symbol<symbol, link<x_h, y_h>, link<x_t, y_t>... >
    {
        using type = typename
            std::conditional_t
            <
                std::is_same<symbol, x_h>::value,
                type::hull<link<x_h, y_h> >,
                resolve_symbol<symbol, link<x_t, y_t>... >
            >::type;

        template<typename... X>
        using with = typename extract_subtype_template_from<type>::template apply_to<X...>;
    };


    template<typename Linker>
    struct add_linker_visibility
    {
        using linker = Linker;
    };
}


namespace aml::adt
{    
    template<template<typename...> class Adt>
    struct trivially_linked
    {
        template<typename, typename... Args>
        using t = Adt<Args...>; 
    };

    
    template<typename...> struct linker;


    template<typename... > struct signatures;


    template<typename... Symbol, template<typename...> class... Subtype>
    struct signatures< link<Symbol, Subtype>... >
    {
        template<typename... X>
        using link_with = typename linker< signatures< link<Symbol, Subtype>... >, X...>::algebraic_type;
    };

    
       
    template
    <
        typename...                    Symbol,
        template<typename...> class... Subtype,
        typename...                    Parameters
    >
    struct linker< signatures< link<Symbol, Subtype>... >, Parameters... >    
    {
    private:
        using this_linker = linker< signatures< link<Symbol, Subtype>... >, Parameters... > ;
    public:        
        template<typename Symbol_, typename... Args>
        using subtype = typename dtl::linker::resolve_symbol<Symbol_, link<Symbol, Subtype>... >::template with<Args...>;


        struct algebraic_type
        :
            public Subtype<this_linker, Parameters... >... ,
            public dtl::linker::add_linker_visibility<linker< signatures< link<Symbol, Subtype>... >, Parameters... > >
        {
            template<typename Symbol_>
            using subtype = typename this_linker::template subtype<Symbol_, Parameters...>;

            constexpr algebraic_type() = default;

            template<typename Other>
            constexpr algebraic_type(Other&& other)
            :
                Subtype< this_linker, Parameters... >(static_cast<Other&&>(other))...
            { }

            template<typename... Args, typename = std::enable_if_t< (sizeof...(Args) > 1) > >
            constexpr algebraic_type(Args&&... args)
            :
                Subtype< this_linker, Parameters... >(static_cast<Args&&>(args))...
            { }

            constexpr algebraic_type& operator=(algebraic_type const&) = default;
            constexpr algebraic_type& operator=(algebraic_type&&)      = default;                

            
            template<typename Symbol_>
            constexpr auto const& csubref() const { return static_cast< subtype<Symbol_> const& >(*this); }

            
            template<typename Symbol_>
            constexpr auto const& subref() const { return csubref(); }


            template<typename Symbol_>
            constexpr auto& subref() { return static_cast< subtype<Symbol_>& >(*this); }
        };

        
        template<typename... X>
        using abstract_type = typename linker< signatures< link<Symbol, Subtype>... >, X... >::algebraic_type;
    };

}
