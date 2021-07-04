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
#include "./lazy_evaluation.hpp"
#include "./logic.hpp"

namespace aml
{
    template < typename... Elements >
    struct set
    :
        private lazy< Elements >...
    {
    private:

        template< typename... >
        friend struct set;

        template < typename X
                 , typename = decltype(static_cast<lazy<X>*> (static_cast< set< Elements... >* >(nullptr)))
                 >
        static true_ contains_element_(decltype(nullptr), decltype(nullptr));


        template < typename X >
        static false_ contains_element_(decltype(nullptr), ... );


        template < typename... X >
        struct contains_
        {
            using type = all< decltype(contains_element_< X >(nullptr, nullptr))... >;
        };

        template<typename...>
        friend class add_elements_to_set_;

        template < typename X >
        static aml::list<Elements..., X> add_element_(decltype(nullptr), ... );


        template< typename X
                , typename  =  typename decltype( contains_element_<X>(nullptr, nullptr) )::sfinae
                >
        static aml::list< Elements... > add_element_(decltype(nullptr), decltype(nullptr) );


        template < typename... Y >
        struct add_elements_to_set_
        {

            template < typename List
                       , typename X
                       >
            using add_element_to_list_rep  =   decltype( List::
                                                                 template apply< set >::
                                                                 template add_element_< X >(nullptr, nullptr) ); //::list;


            using type  =  typename aml::list<Y...>::
                           template lfold_with< add_element_to_list_rep, aml::list<Elements...> >::
                           template apply< set >;
        };


    public:

        static constexpr auto size() { return sizeof...(Elements); }

        constexpr set()            =  default;
        constexpr set(set const&)  =  default;


        using list = aml::list< Elements... >;


        template < typename... Y >
        using add_elements  =  typename add_elements_to_set_<Y...>::type;


        template< typename... X >
        using contains  =  typename contains_< X... >::type;


        template < typename... X >
        using contains_any_of = any< contains< X >... >;


        template < typename... X >
        using contains_none_of = none< contains< X >... >;


        template<  template< typename... > class Pred  >
        using subset_by_predicate  =  typename list::
                                      template partition_with< Pred >::
                                      accepted::
                                      template apply< set >;


        template< typename... X >
        using remove_elements  =  subset_by_predicate<

                predicates::none<    curry_and_bind< is_same, X >::template apply_to...    >::template apply_to

                                                     >;

        // intersection
        template<typename... Y>
        auto operator&(set< Y... >)    ->    subset_by_predicate< set<Y...>::template contains >;


        template< typename... Y >
        auto operator-(set< Y... >)    ->    subset_by_predicate< set<Y...>::template contains_none_of >;


        // union
        template< typename... Y >
        auto operator|(set< Y... >)    ->    typename set< Elements... >::template add_elements< Y... >;


        template < typename... Y >
        constexpr bool operator<=(set< Y... >)
        {
            return set< Y... >::template contains< Elements... >::eval();
        }


        template < typename... Y >
        constexpr bool operator>=(set< Y... > other)
        {
            return other <= *this;
        }


        template < typename... Y >
        constexpr bool operator==(set< Y... > other)
        {
            return *this <= other  &&  other <= *this;
        }


        template < typename... Y >
        constexpr bool operator!=(set< Y... > other)
        {
            return ! (*this == other );
        }


        template < typename... Y >
        constexpr bool operator<(set< Y... > other)
        {
            return ! (*this >= other);
        }


        template < typename... Y >
        constexpr bool operator>(set< Y... > other)
        {
            return !(*this <= other);
        }
    };

}
