#include "./basic_types.hpp"
#include "./partition.hpp"
#include "./curry.hpp"
#include "./find.hpp"


namespace aml
{
    template<typename... >
    struct dictionary;

    template<>
    struct dictionary<>
    {
        using keys     =  conslist<>;
        using values   =  conslist<>;
        using entries  =  conslist<>;

        static constexpr auto size() { return 0; }

        template< typename... Keys >
        using contains_all_keys  =  bool_< sizeof...(Keys) == 0 >;


        template< typename... Zero_Arguments >
        using subdictionary  =  typename enable< dictionary<> >::
                                template if_< bool_< sizeof...(Zero_Arguments) == 0 > >;

    private:

        template< typename >
        using no_value_  =  conslist<>;

    public:

        template< typename... Key >
        using partial_lookup = conslist< no_value_< Key >... >;


        template< typename... Zero_Keys >
        using lookup = typename conslist<>::template check< sizeof...(Zero_Keys) == 0 >;


        template< typename... Entries >
        using add_entries  =  dictionary< Entries... >;


        template< typename... >
        using partially_erase_keys  =  dictionary<>;


        template< typename... No_Keys >
        using erase_keys  =  typename enable< dictionary<> >::
                             template if_<
                                            bool_< sizeof...(No_Keys) == 0 &&
                                                   "The keys to be erasesd are not contained in "
                                                   "the (empty) dictionary" >
                                         >;

    private:

        template< typename... >
        friend struct dictionary;


        template< typename Entry >
        struct fix_entry
        {
            template< typename... >
            struct add_or_replace_in_list;

            template< typename... Entries >
            struct add_or_replace_in_non_empty_list_
            {
                struct replace_head
                {
                    using type  =  typename conslist< Entries... >::tail::template cons< Entry >;
                };


                struct modify_tail
                {
                    using type  =  typename conslist< Entries... >::
                                   tail::
                                   template apply< add_or_replace_in_list >::
                                   type::
                                   template cons< head< conslist< Entries... > > >;
                };

                using key_matches_with_head  =  is_same< key< head< conslist< Entries...> > >, key<Entry> >;

                using type  =  typename key_matches_with_head::template conditional< replace_head, modify_tail >::
                               type;
            };

            struct add_to_empty_list
            {
                using type  =  conslist<Entry>;
            };


            template<typename... Entries>
            struct add_or_replace_in_list
            {
                using list_has_entries  =  bool_<sizeof...(Entries) != 0 >;

                using type = typename list_has_entries::template conditional
                                                                 <
                                                                     add_or_replace_in_non_empty_list_<Entries...>,
                                                                     add_to_empty_list
                                                                 >::type;
            };

        };


        template< typename Entry
                , typename List
                >
        using add_entry_to_list  =  typename List::
                                    template apply< fix_entry< Entry >::template add_or_replace_in_list >::
                                    type;
    };


    template< typename... Entries >
    struct dictionary
    :
        private hull< key< Entries > >...
    {
    private:

        template< typename... Key >
        struct key_list
        {
            template<typename E>
            using entry_matches_with_a_key  = one< is_same< Key, typename E::key >... > ;
        };

        template<typename Key>
        using key_matches_with_an_entry  =  one< is_same< Key, typename Entries::key >... >;


        template<int n, typename List>
        using length_checked  =  typename enable<List>::template if_< bool_<List::size() == n &&
                                                                      "Every argument of at must be contained "
                                                                      "in the dictionary!" > >;

    public:
        using keys     =  conslist<aml::key<Entries>...>;
        using values   =  conslist<aml::value<Entries>...>;
        using entries  =  conslist<Entries...>;


        static auto constexpr size() { return sizeof...(Entries); }


        template< typename... Key >
        using contains_all_keys  =  all< key_matches_with_an_entry<Key>... >;


        template<typename... Keys>
        using subdictionary  =  typename partition<Entries...>::
                                template with< key_list<Keys...>::template entry_matches_with_a_key >::
                                accepted::
                                template apply<dictionary>;


        template<typename... Key>
        using partial_lookup  =  conslist<    typename find<
                                                                composition
                                                                <
                                                                    curry< 1, is_same >::template apply_to< Key >::template apply_to,
                                                                    key
                                                                >::template apply_to
                                                           >::
                                              template in<Entries...>::
                                              template pointwise_apply< value >...  >;


        template< typename... Keys >
        using lookup  =  typename partial_lookup< Keys... >::
                         template apply< join >::
                         template check<    partial_lookup<Keys...>::template apply<join>::size() == sizeof...(Keys) &&
                                            "Not every key has been found in the dictionary!">;



        template< typename... More_Entries >
        using add_entries  =  typename conslist< More_Entries... >::
                              reverse::                  // temporary remedy, should be fixed in the fold
                              template rfold_with< dictionary<>::template add_entry_to_list,   entries >::
                              template apply< dictionary >;


        template<typename... Keys>
        using partially_erase_keys  =  typename partition< Entries... >::
                                       template with< key_list< Keys... >::template entry_matches_with_a_key >::
                                       rejected::
                                       template apply<dictionary>;


        template<typename... Keys>
        using erase_keys  =  typename partially_erase_keys<Keys...>::
                             parameters::
                             template check<    partially_erase_keys<Keys...>::parameters::size() == size() - sizeof...(Keys) &&
                                                "Some keys to be erased are not contained in the dictionary."    >::
                             template apply< dictionary >;
    };
}
