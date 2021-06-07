#include "aml/dictionary.hpp"
#include "aml/exponent.hpp"


namespace aml::compiler::tokenization
{

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


    template
    <
        typename Input,
        typename Automaton,
        typename Sentinel   =  typename Automaton::sentinel,
        typename TokenType  =  Sentinel,
        typename Scanned    =  conslist<>,
        typename Output     =  conslist<>
    >
    struct lexer_step;


    template
    <
        typename Automaton,
        typename Sentinel,
        typename TokenType,
        typename Scanned,
        typename Output
    >
    struct lexer_step
    <
        conslist<>,  // empty input
        Automaton,
        Sentinel,
        TokenType,   // not sentinel
        Scanned,
        Output
    >
    {
        static_assert( is_same<TokenType, Sentinel>::eval() == false, "");

        using type  =  lexer_step<  conslist<>,    Automaton,    Sentinel,    Sentinel,    conslist<>,    typename Output::template rcons< token<erratic_token, Scanned> > >;
    };


    template
    <
        typename Automaton,
        typename Sentinel,
        typename Scanned,
        typename Output
    >
    struct lexer_step
    <
        conslist<>,
        Automaton,
        Sentinel,
        Sentinel,
        Scanned,
        Output
    >
    {
        static_assert( is_same<Scanned, conslist<> >::eval(), "" );

        using type    =  lexer_step< conslist<>,    Automaton,    Sentinel,    Sentinel,    Scanned,    Output>;
        using output  =  Output;
    };


    template
    <
        typename Input,
        typename Automaton,
        typename Sentinel,
        typename TokenType,
        typename Scanned,
        typename Output
    >
    struct lexer_step
    {
    private:
        static_assert( Input::size() > 0, "");

        using next_node   =  typename Automaton::
                             transition_map::
                             template lookup<TokenType>::
                             head::                     // type of head: state<Node, TargetList>
                             template step<typename Input::head>;

        using input_head  =  head<Input>;
        using input_tail  =  tail<Input>;


        struct write_out
        {
            using updated_output = typename Output::template rcons<token<TokenType, Scanned> >;

            using type  =  typename lexer_step< Input,    Automaton,    Sentinel,    Sentinel,     conslist<>,    updated_output >::type;
        };


        struct scan_next
        {
            using updated_scan = typename Scanned::template rcons<input_head>;

            using type  =  typename lexer_step< input_tail,    Automaton,    Sentinel,    next_node,    updated_scan,    Output >::type;
        };


    public:
        using type  =  typename is_same<next_node, Sentinel>::
                       template conditional<write_out, scan_next >::
                       type;

    };


    template<typename Automaton, typename Input>
    using lexer  = lexer_step<Input, Automaton>;
        //typename lexer_step<Input, Automaton>::type;

}


using namespace aml::compiler::tokenization;

struct two {};
struct three {};
struct other {};
struct sentinel {};


template<typename X>
struct is_two
{
    static constexpr bool eval() { return X::eval() == 2; };
};


template<typename X>
struct is_three
{
   static constexpr bool eval() { return X::eval() == 3; };
};




template<typename X>
struct is_other
{
    static constexpr bool eval()
    {
        return X::eval() != 2 && X::eval() != 3;
    }
};


using sentinel_arrows    = arrows<sentinel, target<two, is_two>,      target<three, is_three> >;
using node_two_arrows    = arrows<two,      target<two, is_two>,      target<sentinel, is_three> >;
using node_three_arrows  = arrows<three,    target<three, is_three>,  target<sentinel, is_two> >;


using automaton_t  =  automaton< sentinel_arrows, node_two_arrows, node_three_arrows >;

#include "aml/string.hpp"

using aml::operator""_;

template<auto... objects>
using make_type_list = aml::conslist< aml::object<objects>... >;

using listing = decltype("22323333222"_)::apply<make_type_list>;

using s = typename automaton_t::sentinel;

using tokenized = lexer<automaton_t, listing>;


#include <boost/core/demangle.hpp>
#include <iostream>

int main()
{
    automaton_t::sentinel* ptr;
    //    std::cout << boost::core::demangle(typeid(tokenized).name()) << std::endl;
    
}
