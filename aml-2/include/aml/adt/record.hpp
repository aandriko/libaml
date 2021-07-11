/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "../dictionary.hpp"
#include <functional>

#pragma once

namespace aml::adt
{
    template<  typename Index
            ,  typename Type
            >
    struct indexed_type
    {
    private:
        using this_type = indexed_type<Index, Type>;

        Type type_;

    public:

        template<  typename... Args
                ,  typename  =  typename  // exclude copy and move construction
                                none<  is_same<  list< Args... >, list< this_type const & >  >
                                    ,  is_same<  list< Args... >, list< this_type & >  >
                                    ,  is_same<  list< Args... >, list< this_type && > >
                                    >::sfinae
                >
        explicit constexpr indexed_type( Args&&... args )  noexcept( noexcept( Type( static_cast<Args&&>(args)... ) ) )
        :
            type_( static_cast< Args&& >(args)... )
        { }


        constexpr
        indexed_type (indexed_type const& other )  noexcept( noexcept( Type( other.type_) ) )
        :    type_( other.type_ )
        { }


        constexpr
        indexed_type( indexed_type&& other )  noexcept( noexcept( Type( static_cast< Type&& >( other.type_) ) ) )
        :     type_( static_cast< Type&& >(other.type_) )
        { }

        constexpr indexed_type& operator=( indexed_type const& )   =  default;

        constexpr indexed_type& operator=( indexed_type&&  other)  =  default;


        Type const &  cref() const  noexcept  { return type_; }
        Type const &  ref()  const  noexcept  { return cref(); }
        Type &        ref()         noexcept  { return type_; }
        Type &&       rref()        noexcept  { return static_cast<Type &&>(type_); }
    };


    template<  typename... Entry  >
    struct record
    :    private indexed_type< typename Entry::key, typename Entry::value>...
    {
    private:
        using lookup_table  =  dictionary<Entry...>;

        static_assert( lookup_table::size() == sizeof...(Entry) );

        template<typename Entry_>
        using subtype  =  indexed_type< typename Entry_::key, typename Entry_::value >;

        using this_type = record< Entry... >;

    public:

        template< template<typename... > class Less >
        using reorder_with  =  typename list<Entry...>::
                               template sort_with< Less >::
                               template apply<record>;


        template<  typename... Args
                ,  typename  =  typename  // exclude copy and move construction
                                none<  is_same<  list< Args... >, list< this_type const & >  >
                                    ,  is_same<  list< Args... >, list< this_type & >  >
                                    ,  is_same<  list< Args... >, list< this_type && > >
                                    >::sfinae
                >
        explicit constexpr record( Args&&... args )
            noexcept( (noexcept(subtype<Entry>(static_cast< Args&& >(args) ) ) && ... ) )
        :  subtype<Entry>( static_cast< Args&& >(args)) ...
        { }

        constexpr record() = default;

        constexpr record( record const& )           =  default;

        constexpr record( record && other)  = default;
            /*
            noexcept( (noexcept(subtype<Entry>(static_cast< subtype<Entry>&& >(other) ) ) && ... ) )
        :    subtype<Entry>( static_cast<subtype<Entry> &&>(other) )...
        { }
            */

        constexpr record& operator=(record const&)  =  default;
        constexpr record& operator=(record&& )      =  default;


        template< typename Key >
        constexpr auto const& cref() const  noexcept
        {
            using value_t   =  typename lookup_table::template lookup< Key >;
            using return_t  =  subtype<  entry< Key, value_t >  > const &;

            return static_cast< return_t >(*this).cref();
        }


        template< typename Key >
        constexpr auto const& ref() const  noexcept { return cref<Key>(); }


        template< typename Key >
        constexpr auto& ref()  noexcept
        {
            using value_t  =  typename lookup_table::template lookup< Key >;
            using return_t = subtype<  entry< Key, value_t >  > &;

            return static_cast< return_t >(*this).ref();
        }


        template< typename Key >
        constexpr auto&& rref()  noexcept
        {
            using value_t  =  typename lookup_table::template lookup< Key >;
            using return_t =  subtype<  entry< Key, value_t >  > &&;

            return static_cast< return_t >(*this).rref();
        }


        template< typename F >
        decltype(auto) const_invoke(F&& f) const
            noexcept( noexcept(std::invoke( std::declval<F>(), cref<typename Entry::key>()... ) ) )
        {
            return std::invoke( std::forward<F>(f), cref<typename Entry::key>()... );
        }


        template< typename F >
        decltype(auto) move_invoke(F&& f)  const
            noexcept( noexcept(std::invoke( std::declval<F>(),  rref<typename Entry::key>()... ) ) )
        {
            return std::invoke( std::forward<F>(f), rref<typename Entry::key>()... );
        }


        friend constexpr bool operator==( record<Entry...> const& lhs,
                                          record<Entry...> const& rhs )
        {
            return ( (lhs.cref<typename Entry::key>() == rhs.cref<typename Entry::key>()) && ... );
        }


        friend constexpr bool operator!=( record<Entry...> const& lhs,
                                          record<Entry...> const& rhs )
        {
            return ( (lhs.cref<typename Entry::key>() != rhs.cref<typename Entry::key>()) && ...  && (sizeof...(Entry) != 0 ) );
        }


        friend constexpr bool operator<=( record<Entry...> const& lhs,
                                          record<Entry...> const& rhs )
        {
            return ( (lhs.cref<typename Entry::key>() <= rhs.cref<typename Entry::key>()) && ...  );
        }



        friend constexpr bool operator>=( record<Entry...> const& lhs,
                                          record<Entry...> const& rhs )
        {
            return ( (lhs.cref<typename Entry::key>() >= rhs.cref<typename Entry::key>()) && ... );
        }


        friend constexpr bool operator<( record<Entry...> const& lhs,
                                         record<Entry...> const& rhs )
        {
            return ( (lhs.cref<typename Entry::key>() < rhs.cref<typename Entry::key>()) && ... && (sizeof...(Entry) != 0) );
        }


        friend constexpr bool operator>( record<Entry...> const& lhs,
                                         record<Entry...> const& rhs )
        {
            return ( (lhs.cref<typename Entry::key>() > rhs.cref<typename Entry::key>()) && ... && (sizeof...(Entry) != 0) );
        }
    };
}
