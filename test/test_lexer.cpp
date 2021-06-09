#include "aml/dictionary.hpp"
#include "aml/exponent.hpp"


namespace act //aml::compiler::tokenization
{
    using namespace aml;

    template
    <
        typename                           Target,
        template<typename... Letter> class Source_Target_Predicate
    >
    struct target
    {
        using node = Target;

        template<typename Letter>
        using is_target_for_letter  =  Source_Target_Predicate<Letter>;
    };


    template<typename...>
    struct outgoing_arrows;

    template
    <
        typename...                    Target_Node,
        template<typename...> class... Source_Target_Predicate
    >
    struct outgoing_arrows< target<Target_Node, Source_Target_Predicate>...   >
    {
    private:
        template<typename Letter>
        struct first_satisfied_source_target_predicate
        {
            template<typename Target>
            using apply_to = typename Target::template is_target_for_letter<Letter>;
        };

    public:
        template<typename Letter>
        using step  =  typename find< first_satisfied_source_target_predicate<Letter>::template apply_to >::template in< target<Target_Node, Source_Target_Predicate>... >::head::node;

        using list = conslist<target<Target_Node, Source_Target_Predicate>...>;
    };


    struct erratic_token
    {
    };


    template<typename Node, typename... Target  >
    using arrows = entry<Node, outgoing_arrows<Target... > >;


    template<typename...>
    struct automaton;


    template
    <
        typename... Source_Node,
        typename... Outgoing_Arrows  // = outgoing_arrows<Target...>
    >
    struct automaton<entry<Source_Node, Outgoing_Arrows>... >
    {
    private:
        template<typename Outgoing_Arrows_>
        using add_arrow_to_erratic_token_to  =  typename Outgoing_Arrows_::
                                                list::
                                                template rcons< target<erratic_token, top > >::
                                                template apply<outgoing_arrows>;

        template<typename Sentinel>
        using arrow_from_erratic_token_to  =  arrows<erratic_token,    outgoing_arrows<target<Sentinel, top> > >;

    public:
        using sentinel        =  head< conslist<Source_Node...> >;

        using transition_map  =  typename conslist< entry< Source_Node, add_arrow_to_erratic_token_to<Outgoing_Arrows> >... >
                                 ::template rcons<arrow_from_erratic_token_to<sentinel> >
                                 ::template apply<dictionary>;


    };


    template<typename Type, typename Value>
    struct token
    {
        using type  = Type;
        using value = Value;
    };


    template<typename Input,
             typename Automaton,
             typename Sentinel  = typename Automaton::sentinel,
             typename TokenType = Sentinel,
             typename Scanned   = conslist<>,
             typename Output    = conslist<>>
    struct lexer_step;


    // parser has finished  correctly
    template<typename Automaton,
             typename Sentinel,
             typename Scanned,
             typename Output>
    struct lexer_step<conslist<>,  Automaton,  Sentinel,  Sentinel,  Scanned,  Output>
    {
        static_assert(Scanned::size() == 0, "Internal Error: Parser appears to be ready, but unprocessed input remains.");
        using type   =  lexer_step<  conslist<>,    Automaton,    Sentinel,    erratic_token,    Scanned,    Output>;
        using output = Output;
    };


    // parser has finished too early:
    template<typename Automaton,
             typename Sentinel,
             typename TokenType, // not sentinel!
             typename Scanned,
             typename Output>
    struct lexer_step<conslist<>,  Automaton,  Sentinel,  TokenType,  Scanned,  Output>
    {
        static_assert( is_same<TokenType, Sentinel>::eval() == false, "");
        using type  =  lexer_step<  conslist<>,    Automaton,    Sentinel,    erratic_token,    Scanned,    Output>;
    };


    // Parser has encountered an erratic token
    template<typename Input,
             typename Automaton,
             typename Sentinel,
             typename Scanned,
             typename Output>
    struct lexer_step<  Input,    Automaton,    Sentinel,    erratic_token,    Scanned,    Output>
    {
        static_assert(Input::size() != 0, "");
        using type  =  lexer_step<  conslist<>,    Automaton,    Sentinel,    Sentinel,    conslist<>,    typename Output::template rcons<token<erratic_token, join<Scanned, Input> > > >;
    };


    // parser has encountered an erratic token and the input is empty
    template<typename Automaton,
             typename Sentinel,
             typename Scanned,
             typename Output
             >
    struct lexer_step<  conslist<>,    Automaton,    Sentinel,    erratic_token,    Scanned,    Output  >
    {
        using type  =  lexer_step<  conslist<>,    Automaton,    Sentinel,    erratic_token,    Scanned,    typename Output::template rcons<token<erratic_token, Scanned> > >;
    };


    template<  typename Input,
               typename Automaton,
               typename Sentinel,
               typename TokenType,
               typename Scanned,
               typename Output  >
    struct lexer_step
    {
    private:
        static_assert( Input::size() > 0, "");
        static_assert( is_same<TokenType, erratic_token>::eval() == false, "" );

        using next_node   =  typename Automaton::
                             transition_map::
                             template lookup<TokenType>::
                             head::                     // type of head: outgoing_nodes<target<a_node, a_predicate>... >
                             template step<typename Input::head>;

        using input_head  =  head<Input>;
        using input_tail  =  tail<Input>;


        struct write_out
        {
            using updated_output = typename Output::template rcons<token<TokenType, Scanned> >;

            using type  =  lexer_step< Input,    Automaton,    Sentinel,    Sentinel,     conslist<>,    updated_output >;
        };


        struct scan_next
        {
            using updated_scan = typename Scanned::template rcons<input_head>;

            using type  =  lexer_step< input_tail,    Automaton,    Sentinel,    next_node,    updated_scan,    Output >;
        };


    public:
        using type  =  typename is_same<next_node, Sentinel>::
                       template conditional<write_out, scan_next >::
                       type;

    };


    template<typename Automaton, typename Input>
    using lexer  =  power< exp<1>, lexer_step<Input, Automaton> >;

        //lexer_step<Input, Automaton>;
        //typename lexer_step<Input, Automaton>::type;

}


//using namespace aml::compiler::tokenization;
using namespace act;


struct one_t;
struct semicolon_t;
struct sentinel_t;


template<typename X>
struct is_one
{
    static constexpr bool eval() { return X::eval() == '1'; };
};


template<typename X>
struct is_semicolon
{
    static constexpr bool eval() { return X::eval() == ';'; };
};



using sentinel_arrows        = arrows<sentinel_t,  target<one_t, is_one>,       target<semicolon_t, is_semicolon> >;
using node_one_arrows        = arrows<one_t,       target<one_t, is_one>,       target<sentinel_t,  is_semicolon> >;
using node_semicolon_arrows  = arrows<semicolon_t, target<sentinel_t, is_one>,  target<sentinel_t,  is_semicolon> >;


using automaton_t  =  automaton< sentinel_arrows, node_one_arrows, node_semicolon_arrows >;

#include "aml/string.hpp"

using aml::operator""_;

template<auto... objects>
using make_type_list = aml::conslist< aml::object<objects>... >;

//using listing = decltype("22323333222"_)::apply<make_type_list>;
using listing = decltype("1;"_)::apply<make_type_list>;

using s = typename automaton_t::sentinel;

using tokenized = lexer<automaton_t, listing>;


#include <boost/core/demangle.hpp>
#include <iostream>

int main()
{
    std::cout << boost::core::demangle(typeid(tokenized).name()) << std::endl;
    //    automaton_t::sentinel* ptr;
    //    std::cout << boost::core::demangle(typeid(tokenized).name()) << std::endl;
    
}
