/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "aml/basic_types.hpp"
#include "aml/find.hpp"


namespace aml
{
    template < typename... Elements >
    struct set
    :
        private hull< Elements >...
    {
    private:

        template < typename X
                 , typename = decltype(static_cast<hull<X>*> (static_cast< set< Elements... >* >(nullptr)))
                 >
        static true_ contains_element_(decltype(nullptr), decltype(nullptr));


        template < typename X >
        static false_ contains_element_(decltype(nullptr), ... );


        template < typename... X >
        struct contains_
        {
            using type = all< decltype(contains_element_< X >(nullptr, nullptr))... >;
        };


        template < typename X
                 , typename R = set< Elements..., X >
                 >
        static R add_element_(decltype(nullptr), decltype(nullptr));


        template < typename X >
        static set< Elements... > add_element_(decltype(nullptr), ... );


        template < typename List
                 , typename X
                 >
        using add_element_to_list_rep  =  typename decltype( List::
                                                             template apply< set >::
                                                             template add_element_< X >(nullptr, nullptr) )
                                                   ::list;

    public:

        static constexpr auto size() { return sizeof...(Elements); }


        using list = conslist< Elements... >;


        template < typename... Y >
        using add_elements  =  typename conslist< Y... >::
                               template lfold_with< add_element_to_list_rep, list >::
                               template apply< set >;


        template< typename... X >
        using contains = all< typename contains_< X >::type... >;


        template < typename... X >
        using contains_one_of = one< contains< X >... >;


        template < typename... X >
        using contains_none_of = none< contains< X >... >;


        template< template<typename...> class Pred >
        using subset_by_predicate  =  typename find< Pred >::
                                      template in< Elements... >::
                                      accepted::
                                      template apply< set >;

        // intersection
        template< typename... Y >
        auto operator&(set< Y... >)    ->    typename find< set< Elements... >::template contains >::
                                             template in< Y... >::
                                             accepted::
                                             template apply< set >;


        // union
        template< typename... Y >
        auto operator|(set< Y... >)    ->    typename set< Elements... >::template add_elements< Y... >;


        template< typename... Y >
        auto operator-(set< Y... >)    ->    typename find< set< Elements... >::template contains >::
                                             template in< Y... >::
                                             rejected::
                                             template apply< set >;


        template < typename... Y >
        auto operator<=(set< Y... >)    ->    typename set< Y... >::
                                              template contains< Elements... >;


        template < typename... Y >
        auto operator>=(set< Y... >)    ->    typename set< Elements... >::
                                              template contains< Y... >;


        template < typename... Y >
        auto operator==(set< Y... >)    ->    all
                                              <
                                                  typename set< Y... >::template contains< Elements... >,
                                                  typename set< Elements... >::template contains< Y... >
                                              >;


        template < typename... Y >
        auto operator!=(set< Y... >)    ->    all
                                              <
                                                  none< typename set< Y... >::template contains< Elements... > >,
                                                  none< typename set< Elements... >::template contains< Y... > >
                                              >;


        template < typename... Y >
        auto operator<(set< Y... >)    ->     all
                                              <
                                                  typename set< Y... >::template contains< Elements... >,
                                                  none<typename set< Elements... >::template contains< Y... > >
                                              >;


        template < typename... Y >
        auto operator>(set< Y... >)    ->    all
                                             <
                                                 typename set< Elements... >::template contains< Y... >,
                                                 none< typename set< Y... >::template contains< Elements... > >
                                             >;

    };

}
