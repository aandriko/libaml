#pragma once

#include <type_traits>
#include <utility>

#include "./parameters.hpp" 
#include "./conslist.hpp"


namespace aml
{
    template<typename...> struct signatures;

    template<typename Symbol, template<typename...> class SubType>
    struct subtype
    {
    private:
        template<typename... X>
        using ignore_linker_in_subtype = typename conslist<X...>::tail::template apply<SubType>;

        friend struct signatures<>;
        
        template<typename... X>
        using function = SubType<X...>;

        using symbol = Symbol;
        
    public:
        using ignore_linker = subtype<Symbol, ignore_linker_in_subtype>;
    };
}


namespace aml::dtl::linker
{

    template<typename T>
    struct extract_subtype_template_from;

    
    template<typename Symbol, template<typename...> class SubType>
    struct extract_subtype_template_from<subtype<Symbol, SubType> >
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

    
    template
    <
        typename Linker,
        typename symbol,
        typename    x_h, template<typename...> class    y_h,
        typename... x_t, template<typename...> class... y_t
    >
    struct resolve_symbol<Linker, symbol, aml::subtype<x_h, y_h>, aml::subtype<x_t, y_t>... >
    {
        using type = typename
            std::conditional_t
            <
                std::is_same<symbol, x_h>::value,

                type::hull<aml::subtype<x_h, y_h> >,

                resolve_symbol<symbol, aml::subtype<x_t, y_t>... >
            >::type;

        template<typename... X>
        using with = typename extract_subtype_template_from<type>::template apply_to<Linker, X...>;
    };


    template<typename Linker>
    struct add_linker_visibility
    {
        using linker = Linker;
    };
}


namespace aml
{        
    template<typename...> struct linker;


    template<typename... Signatures>
    struct signatures
    {
        template<typename... X>
        using link_with = typename
            linker
            <
                signatures<Signatures... >,
                X...
            >::algebraic_type;
    };

    template<>
    struct signatures
    {
        //        template<typename Symbol_, typename Subtype>
        //        using 
    };
       
    template
    <
        typename...                    Symbol,
        template<typename...> class... Subtype,
        typename...                    Parameters
    >
    struct linker< signatures< aml::subtype<Symbol, Subtype>... >, Parameters... >    
    {
    private:
        using this_linker = linker< signatures< aml::subtype<Symbol, Subtype>... >, Parameters... > ;
    public:        
        template<typename Symbol_, typename... Args>
        using subtype = typename dtl::linker::resolve_symbol<this_linker, Symbol_, aml::subtype<Symbol, Subtype>... >::template with<Args...>;


        struct algebraic_type
        :
            public Subtype<this_linker, Parameters... >... ,
            public dtl::linker::add_linker_visibility<this_linker>
        {
            using structure = this_linker;

            
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
        using abstract_type = typename linker< signatures< aml::subtype<Symbol, Subtype>... >, X... >::algebraic_type;
    };

}
