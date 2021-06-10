#include "aml/dictionary.hpp"
#include "aml/exponent.hpp"
#include "aml/set.hpp"


namespace aml::compiler::tokenization
{

    template<typename Node, template<typename...> class Label_Predicate>
    struct target;


    template< typename... >
    struct arrows;


    template< typename                       Source_Node
            , typename...                    Target_Node
            , template<typename...> class... Label_Predicate
            >
    struct arrows<Source_Node, target<Target_Node, Label_Predicate>... >
    {
        //    private:
        template<typename...>
        friend struct dfa;


        using source_node  =  Source_Node;


        template<typename Label>
        struct bind
        {
            template<typename Encoded_Target>
            using target_belongs_to_label  =  typename Encoded_Target::key::template apply_to<Label>;
        };

        // target_node<Label> = conslist<Node> or conslist<> if not found
        template<typename Label>
        using target_node  =  typename find
                                  <
            bind<Label>::template target_belongs_to_label
            //            curry<1, label_matches_with_target>::template apply_to<Label>::template apply_to

                                  >::
                                  template in< entry< function< Label_Predicate >, Target_Node >... >::
                                  template pointwise_apply< value >;
    };



    template<typename...>
    struct dfa;


    template< typename     Start
            , typename     Acceptor_Set
            , typename...  Arrows
            >
    struct dfa< Start, Acceptor_Set, Arrows... >
    {
    private:
        using transition_map  =  dictionary< entry<typename Arrows::source_node, Arrows>... >;

        struct no_node_found
        {
            template<typename>
            using target_node = conslist<>;
        };

        template< typename Label >
        struct fix_label
        {
            template< typename Arrows_ >
            using target_node = typename Arrows_::template target_node<Label>;
        };

    public:

        using start      =  Start;
        using acceptors  =  Acceptor_Set;

        template< typename Node
                , typename Label
                >
        using transition  =  typename transition_map::
                             template partial_lookup<Node>::
                             head::
                             template pointwise_apply< fix_label<Label>::template target_node >;

        // transition<N, L> == conslist<P>, iff N-->P is the first edge to satisfy the label L.
        // transition<N, L> == conslist<>,  iff such and edge does not exist.
    };


    template< typename Type
            , typename Value
            >
    struct token
    {
        using type  = Type;
        using value = Value;
    };


    struct unfinished_token;


    template< typename Token
            , typename Value
            >
    struct token_error
    {
        using type   =  Token;
        using value  =  Value;
    };


    template< typename dfa
            , typename Input
            , typename Node
            , typename Scanned
            , typename Output
            >
    struct lexer_step
    {
        // 1) Input == conslist<> and Scanned != conslist<> indicates an error (unfinished token), abnormal termination
        // 2) Input == conslist<> and Scanned == conslist<> is the condition of normal termination
        // 3) Input != conslist<> indicates that the parser is runing:
        //
        //      subcase 3a) no transition exists and the state "Node" is conctained in dfa::acceptors:
        //                  token is written to output
        //      subcase 3b) no transition exists and the state "Note" is not contained in dfa::acceptors:
        //                  The token token_error<Node, Scanned> is written to output.
        //      subcase 3c) Node is a non-final state and the input stream is not empty. Then the Scanned is updated.

        struct terminate_with_unfinished_token
        {
            using output_  =  typename Output::template rcons<token_error<unfinished_token, Scanned > >;

            using type = lexer_step< dfa, conslist<>, typename dfa::start, conslist<>, output_ >;
        };

        struct terminate
        {
            //            using type   = lexer_step<dfa, conslist<>, dfa::start, conslist<>, Output>;
            using output  =  Output;
        };

        struct write_token_error
        {
            using output_  =  typename Output::template rcons< token_error< Node, Scanned > >;

            using type = lexer_step<dfa, Input, typename dfa::start, conslist<>, output_>;
        };


        struct write_valid_token
        {
            using output_  =  typename Output::template rcons< token< Node, Scanned > > ;

            using type = lexer_step<dfa, Input, typename dfa::start, conslist<>, output_>;
        };


        struct consume_symbol
        {
            using symbol_   =  head< Input >;
            using node_     =  typename dfa::template transition< Node, symbol_ >::head;
            using input_    =  tail<Input>;
            using scanned_  =  typename Scanned::template rcons< symbol_ >;

            using type = lexer_step< dfa, input_, node_, scanned_, Output >;
        };


        struct final_step
        {
            using type = typename conditional< bool_<Scanned::size() == 0>,
                                               terminate,
                                               terminate_with_unfinished_token >::type;
        };


        struct intermediate_step
        {
            using symbol_      =  head< Input >;
            using transition_  =  typename dfa::template transition< Node, symbol_ >;

            using type  =  typename conditional
                                    <
                                        typename dfa::acceptors::template contains<Node>,

                                        write_valid_token,

                                        conditional
                                        <
                                            is_same<transition_, conslist<> >,

                                            write_token_error,

                                            consume_symbol
                                        >
                                    >::type;
        };

        using type = typename conditional< bool_< Input::size() == 0 >, final_step, intermediate_step >::type;
    };


    template<typename dfa, typename Input>
    using lexer  =  //typename power< exp<infinity>, lexer_step<dfa, Input, typename dfa::start, conslist<>, conslist<> > >::output;
        typename lexer_step<dfa, Input, typename dfa::start, conslist<>, conslist<> >::type;

}



using namespace aml::compiler::tokenization;

struct start_t;
struct one_t;
struct semicolon_t;


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


using start_arrows = arrows<start_t, target<one_t, is_one>, target<semicolon_t, is_semicolon> >;
using arrows_from_one_t = arrows<one_t, target<one_t, is_one> >;
using arrows_from_semicolon_t = arrows<semicolon_t, target<semicolon_t, is_semicolon> >;

using dfa_t = dfa<  start_t,    aml::set<semicolon_t, one_t>,    start_arrows,
                                                                 arrows_from_one_t,
                                                                 arrows_from_semicolon_t >;


#include "aml/string.hpp"

using aml::operator""_;

template<auto... objects>
using make_type_list = aml::conslist< aml::object<objects>... >;


using listing = decltype("1"_)::apply<make_type_list>;


using tokenized = lexer<dfa_t, listing>;


#include <boost/core/demangle.hpp>
#include <iostream>

int main()
{
    std::cout << boost::core::demangle(typeid(tokenized).name()) << std::endl;

}
