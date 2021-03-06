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
#include <boost/core/demangle.hpp>
#include <iostream>

namespace aml::adt
{
    template<  typename Symbol
            ,  template<typename... > class SubType
            >
    using subtype  =  aml::entry<Symbol, aml::function<SubType> >;


    template<typename... SubType>
    struct signature
    {
    private:


        template<typename... X>
        struct linker
        {
            template<typename Symbol>
            using lookup  =  typename dictionary<SubType...>::
                             template lookup<Symbol>::
                             template apply_to<  linker<X...>,  X... >;

            template<typename Symbol>
            using entry = aml::entry< Symbol, lookup<Symbol> >;
        };


    public:

        template<typename... X>
        struct adt
        :    public record< typename linker<X...>::template entry< typename SubType::key >... >
        {
        private:
            using record_t  =  record<  typename linker<X...>::
                                        template entry< typename SubType::key >...  >;

            struct construct_from_permuted_record {};

            template<typename Record>
            adt(construct_from_permuted_record,  Record&& r)
                : record_t( static_cast<Record&&>(r).template extract< record_t >() )
            { }

        public:

            using signature  =  signature<SubType...>;

            template<typename Symbol>
            using sub  =  typename linker<X...>::template entry<Symbol>;

            adt()            =  default;
            adt(adt const&)  =  default;
            adt(adt&& )      =  default;


            template<  typename... SubAdt,
                       // sfinae away the case when the default copy- or move-constructor shall be called
                       typename  =  typename bool_<    ! std::is_same< list<SubAdt&&... >, list< adt const&   > >::value &&
                                                       ! std::is_same< list<SubAdt&&... >, list< adt &        > >::value &&
                                                       ! std::is_same< list<SubAdt&&... >, list< adt const && > >::value &&
                                                       ! std::is_same< list<SubAdt&&... >, list< adt &&       > >::value
                                                  >::sfinae
                    >
            adt(SubAdt&&... sub_adt)
                : adt( construct_from_permuted_record{}, make_record( static_cast<SubAdt&&>(sub_adt)... ) )
            { }


            record_t const& record() const &   { return static_cast<record_t const&>(*this); }
            record_t&&      record()       &&  { return static_cast<record_t&&>(*this); }
            record_t&       record()       &   { return static_cast<record_t&>(*this); }
            record_t const& crecord() const    { return static_cast<record_t const&>(*this); }

            template< typename T >
            auto const& cref()  const &
            {
                return record().template cref< std::decay_t<T> >();
            }


            template< typename T >
            auto const& ref()  const &
            {
                return cref<T>();
            }


            template< typename T >
            auto& ref()  &
            {
                return record().template ref<T>();
            }


            template< typename T >
            auto&& rref()
            {
                return record().template rref< std::decay_t<T> >();
            }


            template< typename T >
            auto&& ref() &&
            {
                return record().template rref< std::decay_t<T> >();
            }


            friend bool operator==(adt const& lhs, adt const& rhs)
            {
                return lhs.crecord() == rhs.crecord();
            }


            friend bool operator!=(adt const& lhs, adt const& rhs)
            {
                return ! ( lhs == rhs );
            }
        };

    };

}


/*
    

    template<typename... SubType>
    struct link
    {
    private:

        using lookup = dictionary<SubType...>;

        static_assert(lookup::size() == sizeof...(SubType),
                      "Symbols for sub types must be unique!");

        using this_linker     =  link<SubType...>;

    public:



        
        template< typename... X >
        class adt
        :
            public SubType::value::template apply_to< this_linker, X... >...
        {
        private:
            using this_type  =  adt< X... >;


            template< typename SubType_ >
            using symbol  =  typename SubType_::key;


            template< typename SubType_ >
            using instance  =  typename SubType_::value::template apply_to< this_linker, X...>;


            template< typename SubType_ >
            using entry  =  aml::entry<  symbol< SubType_>,  instance< SubType_ > >;


            using data_t  =  record< entry<SubType>... >;


            data_t data_;


        public:
            using signature  =  lookup;


            template<typename Symbol>
            using factor  =  typename signature::template lookup<Symbol>::template apply_to<this_linker, X...>;


            template<  typename... Args
                    ,  typename  =  typename  // exclude copy and move construction
                                    none<  is_same<  list< Args... >, list< this_type const & >  >
                                        ,  is_same<  list< Args... >, list< this_type & >  >
                                        ,  is_same<  list< Args... >, list< this_type && > >
                                        >::sfinae
                >
            explicit adt( Args&&... args )
            :   instance<SubType>(static_cast<Args&&>(args))...
            {}


            adt()
            :   instance<SubType>()...
            { }


            adt( adt const &  other)
            :   instance<SubType>( static_cast< instance<SubType> const & >(other))...
            { }

            adt( adt &&  other)
            :   instance<SubType>(static_cast< instance<SubType>&& >(other))...
            { }


            template< typename S >
            auto const&  operator[](const S&)  const &
            {
                return data_.template cref< std::decay_t<S> >();
            }


            template< typename S >
            auto& operator[](const S&)  &
            {
                return data_.template ref< std::decay_t<S> >();
            }


            template< typename S >
            auto&& operator[](S const&)  &&
            {
                return data_.template rref< std::decay_t<S> >();
            }


            template< typename T >
            auto const& cref()  const &
            {
                return data_.template cref< std::decay_t<T> >();
            }


            template< typename T >
            auto const& ref()  const &
            {
                return cref<T>();
            }


            template< typename T >
            auto& ref()  &
            {
                return ref<T>();
            }


            template< typename T >
            auto&& rref()
            {
                return data_.template rref< std::decay_t<T> >();
            }


            template< typename T >
            auto&& ref() &&
            {
                return data_.template rref< std::decay_t<T> >();
            }

        };


    };

}
*/
