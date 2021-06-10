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
            , typename...  Arrows
            >
    struct dfa< Start, Arrows... >
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


    template< typename dfa
            , typename Input
            , typename Node
            , typename Scanned
            , typename Output
            >
    struct lexer_step
    {
        struct terminate
        {
            using output  =  typename bool_< Scanned::size() != 0 >::
                             template conditional< typename Output::template rcons< token< Node, Scanned > >,  Output >;

            using type    =  lexer_step< dfa, conslist<>, typename dfa::start, conslist<>, output >;
        };


        struct consume_symbol
        {
            using symbol_         = head<Input>;
            using wrapped_node_   = typename dfa::template transition<Node, symbol_>;

            struct write_out
            {
                using output_  =  typename Output::template rcons< token< Node, Scanned > >;
                using type     =  lexer_step< dfa, Input, typename dfa::start, conslist<>, output_ >;
            };

            struct scan
            {
                using node_   = head< wrapped_node_ >;
                using input_  = tail<Input>;
                using scanned_ = typename Scanned::template rcons<symbol_>;

                using type = lexer_step< dfa, input_, node_ , scanned_, Output>;
            };

            using type = typename conditional< is_same<wrapped_node_, conslist<> >,  write_out,  scan >::type;
        };

        using type = typename conditional< bool_<Input::size() == 0>, terminate, consume_symbol>::type;
        using output = Output;
    };


    template<typename dfa, typename Input>
    using lexer  =  typename power< exp<infinity>, lexer_step<dfa, Input, typename dfa::start, conslist<>, conslist<> > >::output;

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

using dfa_t = dfa<  start_t,  start_arrows,  arrows_from_one_t,  arrows_from_semicolon_t >;


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
