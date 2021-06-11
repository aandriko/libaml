#include "aml/dictionary.hpp"
#include "aml/exponent.hpp"
#include "aml/set.hpp"

#ifdef ACTIVATE_TEST_GRAPH

namespace aml::fsm::graph
{
    template<typename...>
    struct edge;


    template<typename Source, typename Target>
    struct edge
    {
        static_assert( is_same< Source, Target >::eval() == false, "Loops of length 0 are forbidden" );

        using source = Source;
        using target = Target;
    };


    template<typename Edge>
    using source  =  typename Edge::source;


    template<typename Edge>
    using target  =  typename Edge::target;


    template< typename... >
    struct arrows;


    template< typename Source
            , typename... Targets
            >
    struct arrows< Source, Targets... >
    {
        using source  = Source;
        using targets = set< Targets... >;
        using edges   = set< edge<source, Targets>... >;
    };


    template< typename... >
    struct graph;


    template<>
    struct graph<dictionary<> >
    {
    private:

        template<typename... Edges>
        struct add_edges_
        {
            static_assert( sizeof...(Edges) != 0, "" );

            using src  =  source< head< conslist<Edges...> > >;
            using trg  =  target< head< conslist<Edges...> > >;

            using type =  typename tail<Edges...>::
                          template apply<graph< dictionary< entry< src, arrows<src, trg> >,
                                                            entry< trg, arrows<trg> > > > >::template add_edges>;
        };

    public:
        using nodes = set<>;
        using edges = set<>;


        template<typename... Node>
        struct outgoing_from
        {
            using error = bool_< sizeof...(Node) != sizeof...(Node) >;
            static_assert(error , "An empty graph has no outgoing nodes" );
        };


        template< typename... Nodes >
        using add_nodes  =  graph< dictionary< entry< Nodes, arrows< Nodes > >... >;


        template<typename... Edges>
        struct add_edges  =  typename bool_< sizeof...(Edges) != 0 >::
                             template conditional< add_edges_<Edges...>, hull< graph<dictionary<>, Base> > >::type;


        template< template<typename...> class>
        using isomorphic_node_transform  =  graph< dictionary<> >;


        template<typename... Nodes>
        using identify_nodes
        {
            static_assert( sizeof...(Nodes) == 0, "Empty dictionary contains no nodes. "
                                                   "Hence the current list is not identifiable. " );
            template<typename... Node>
            using with = typename if_< bool_<sizeof...(Node) == 1> >::enable< graph<dictionary<> >;
        };
    };


    template<typename... Entries>
    struct graph< dictionary< Entries... > >
    {
    private:
        using all_nodes_ = join< typename term<Entries>::subterms... >::template apply<set>;

        static_assert( decltype( all_nodes_() == set< key<Entries>... >() ),
                       "Every node must be explicitly present as an entry");

    public:
        using nodes  =  set< key<Entries...> >;
        using edges  =  typename join< typename value< Entries >::list... >::template apply< set >;


        template<typename... Node>
        using outgoing_from = typename dictionary<Entries...>::template at< identity<Node...> >;


        template<typename... Nodes>
        using add_nodes  =  graph< dictionary<  Entries...,    entry<Nodes, arrows<Nodes> >...  >;


        template<typenane... Edges >
        using add_edges  =  typename
                            term
                            <
                                decltype
                                (
                                    typename
                                    join< conslist< source< Edges >, target< Edges > >... >::template apply<set>()
                                    -
                                    nodes()
                                )::list::template apply< add_node >
                            >::
                            subterms::
                            entries::
                            template apply< update_entries_with<Edges...>::template apply >::
                            template apply< dictionary >::
                            template apply< graph >;


        template< template<typename...> class F>
        using isomorphic_node_transform  =  graph< dictionary< node_transform_<F, Entries>... >;


        template<typename... Nodes>
        struct identify_nodes
        {
            template<typename... New_node>
            struct with;

        }

    };


    template<typename... Arrows>
    using make_graph  =  typename join
                         <
                             conslist< entry< typename Arrows::source, Arrows>... >,

                             typename
                             decltype(
                                        typename join<typename Arrows::targets::list...>::template apply< set > >()
                                        -
                                        set< typename Arrows::source... >()

                                      )::  // Here conslist<node...> with all nodes that do not appear in arrows
                                           // as a soure

                             template pointwise_apply< diagonal >::        // conslist< conslist<node, node>... >
                             emplate pointwise_apply< pointwise_action<identity, arrows> >::
                                                             // Here: conslist< conslist< node, arrows<node>... >
                             template pointwise_apply< entry >
                         >::
                         template apply<dictionary>::
                         template apply<graph>;

}

#endif

#include "aml/string.hpp"

using aml::operator""_;



#include <boost/core/demangle.hpp>
#include <iostream>

int main()
{


}
