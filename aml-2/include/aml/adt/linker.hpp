/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../dictionary.hpp"
#include "../term_algebra.hpp"
#include "./record.hpp"

#include <type_traits>

namespace aml::adt
{
    template<  typename Symbol
            ,  template<typename... > class SubType
            >
    using subtype  =   aml::entry<Symbol, aml::function<SubType> >;


    template<typename X, typename... Args>
    inline constexpr indexed_type<X, record<Args&&... > > arg(Args&&... args)
    {
        return indexed_type<X, record< aml::entry<Args, Args&&>... > >( static_cast<Args&&>(args)... );
    }

    template<typename... SubType>
    struct link
    {
    private:

        using lookup = dictionary<SubType...>;

        static_assert(lookup::size() == sizeof...(SubType),
                      "Symbols for sub types must be unique!");


    public:


        template< typename... X >
        class adt
        {
        private:
            using this_type  =  adt< X... >;

            using data_t  =  record<  aml::entry<  typename SubType::key
                                                ,  typename SubType::value::template apply_to<X...>
                                                >...
                                   >;


            data_t data_;


            struct construct_from_record {};


            template<typename Record>
            adt( construct_from_record, Record&& r)
            //            :
                //                data_t( r.template rref< typename SubType::key >()... )
            { }


            template<typename T>
            using make_entry = typename aml::term<std::decay_t<T> >::subterms::template apply<aml::entry>;

        public:


            template<  typename... Args
                    ,  typename  =  typename  // exclude copy and move construction
                                    none<  is_same<  list< Args... >, list< this_type const & >  >
                                        ,  is_same<  list< Args... >, list< this_type & >  >
                                        ,  is_same<  list< Args... >, list< this_type && > >
                                        >::sfinae
                >

            adt( Args&&... args )
            :    adt( construct_from_record{},
                      record<  make_entry<Args>... >(static_cast< Args && >(args)... ))
            { }


            adt( adt const& )  =  default;
            adt( adt &&     )  =  default;
        };


    };

}
