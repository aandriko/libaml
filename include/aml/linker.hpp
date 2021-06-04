#pragma once

#include "./basic_types.hpp"
#include "./find.hpp"

namespace aml
{
    template<typename...> struct linker;


    template<typename...> struct signatures;


    template<typename Symbol, template<typename...> class SubType>
    struct subtype
    {
    private:
        template<typename... X>
        using ignore_linker_in_subtype = typename conslist<X...>::tail::template apply<SubType>;


        friend  struct signatures<>;


        template<typename Symbol_>
        static constexpr bool is_indexed_by_symbol() { return is_same<Symbol, Symbol_>::eval(); }


    public:
        using ignore_linker = subtype<Symbol, ignore_linker_in_subtype>;


        template<typename... X>
        using resolve_with = SubType<X...>;
    };


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
    struct signatures<>
    {
    private:
        template<typename...>
        friend struct linker;


        template<typename Symbol>
        struct fix_symbol
        {
            template<typename Subtype>
            struct match
            {
                static constexpr bool eval()
                {
                    return Subtype::template is_indexed_by_symbol<Symbol>();
                };
            };
        };


        template<typename... Symbol>
        struct check_symbol_existence
            : public conslist<Symbol...>
        {
            static_assert(sizeof...(Symbol) == 1, "Symbol does not exist");
        };
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
        using subtype =
            typename
            aml::find< signatures<>::fix_symbol<Symbol_>::template match >
               ::template in< aml::subtype<Symbol, Subtype>... >
               ::template apply< signatures<>::template check_symbol_existence >
               ::head
               ::template resolve_with<Args...>;


        struct algebraic_type
        :
            public Subtype<this_linker, Parameters... >...
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

            //            template<typename... Args, typename = typename enable<void>::template if_< (sizeof...(Args) > 1) > >
            template<typename... Args, typename = typename bool_< (sizeof...(Args) > 1) >::sfinae >
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
