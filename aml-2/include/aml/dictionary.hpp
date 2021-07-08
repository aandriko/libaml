/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "./list.hpp"
#include "./set.hpp"

namespace aml
{
    template< typename... >
    struct entry;

    template< typename Key
            , typename Value
            >
    struct entry< Key, Value >
    {
        using key   =  Key;
        using value =  Value;
    };


    template<typename... Sentinel>
    struct sentinel
    {
        static_assert(sizeof...(Sentinel) <= 1);
    };

    template<typename Sentinel>
    struct sentinel<Sentinel>
    {
        using key    =  Sentinel;
        using value  =  Sentinel;
    };


    template<typename... Entries>
    struct dictionary;


    template<  typename... Sentinel
            ,  typename... Key
            ,  typename... Value
            >
    struct dictionary<  sentinel< Sentinel... >, entry< Key, Value >...  >
    :
        private set<Key...>  // ensure that keys are unique
    {
        using key_list    =  aml::list<  Key...  >;
        using value_list  =  aml::list<  Value...  >;
        using entry_list  =  aml::list<  entry< Key, Value >...  >;

        using entry_set   =  aml::set< entry<Key, Value>... >;
        using key_set     =  aml::set< Key... >;

    private:

        template<typename Key_>
        struct key_matches_entry
        {
            template<typename Entry_>
            using apply_to  =  is_same< typename Entry_::key, Key_ >;
        };

    public:

        template< typename... S >
        using with_sentinel  =  dictionary<  sentinel<S...>, entry< Key, Value >...  >;


        template< typename Key_ >
        using at  =  typename entry_list::
                     template split_by_first_occurence_of< key_matches_entry<Key_>::
                                                           template apply_to >::
                     suffix::
                     template rcons< sentinel<Sentinel>...>::
                     head::
                     value;


        template< typename... Key_ >
        using map_to_values  =  aml::list< at<Key_>... >;


    private:

        template<  typename List
                ,  typename Entry
                ,  typename split  =  typename List::
                                      template split_by_first_occurence_of<
                       key_matches_entry< typename Entry::key >::template apply_to >
               >
        using add_entry_to_list  =  typename split::
                                    prefix::
                                    template rcons< Entry >::
            template apply< split::suffix::relaxed_tail::template cons >;


        template<  typename List
                ,  typename Key_
                ,  typename split  =  typename List::
                                      template split_by_first_occurence_of<
                       key_matches_entry< Key_ >::template apply_to >
               >
        using remove_key_from_list  =  typename split::
                                       prefix::
                                       template apply< split::
                                                       suffix::
                                                       relaxed_tail::
                                                       template cons >;


    public:

        template<typename... New_Entries>
        using add_entries  =  typename aml::list< New_Entries... >::
                              template lfold_with< add_entry_to_list, entry_list >::
                              template apply< dictionary >::
                              template with_sentinel< Sentinel... >;



        template<typename... Remove_Keys>
        using remove_keys  =  typename aml::list< Remove_Keys... >::
                              template lfold_with< remove_key_from_list, entry_list >::
                              template apply< dictionary >::
                              template with_sentinel< Sentinel... >;


        static constexpr auto size() { return sizeof...(Key); }

    };

    template<  typename... Key
            ,  typename... Value
            >
    struct dictionary<  entry< Key, Value >...  >
    :
        public dictionary<  sentinel<>, entry< Key, Value >...  >
    { };


}

