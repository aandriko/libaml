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


namespace aml::adt
{
    // indexed_type<Index, Type> is a wrapper that keeps an object of the type Type
    //
    // The kept object can be accessed by reference through the methods ref(), cref() and
    // rref()
    //
    template<  typename Index
            ,  typename Type
            >
    struct indexed_type
    {
    private:
        using this_type = indexed_type<Index, Type>;

        Type type_;

    public:

        using type = Type;

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


        indexed_type(indexed_type const&) = default;

        indexed_type(indexed_type &&) = default;

        template< typename Type_ >
        constexpr indexed_type(indexed_type<Index, Type_> const &  other )  noexcept( noexcept( Type( other.cref() ) ) )
        :  type_( other.cref() )
        { }


        template< typename Type_ >
        constexpr indexed_type(indexed_type<Index, Type_>&&  other )  noexcept( noexcept( Type( other.rref() ) ) )
        :  type_( other.rref() )
        { }


        constexpr indexed_type& operator=( indexed_type const& )   =  default;


        constexpr indexed_type& operator=( indexed_type&&  other)  =  default;


        Type const &  cref() const  noexcept  { return type_; }
        Type const &  ref()  const  noexcept  { return cref(); }
        Type &        ref()         noexcept  { return type_; }
        Type &&       rref()        noexcept  { return static_cast<Type &&>(type_); }

        friend bool operator==(indexed_type const& lhs, indexed_type const& rhs)
        {
            return lhs.cref() == rhs.cref();
        }

        friend bool operator!=(indexed_type const& lhs, indexed_type const& rhs)
        {
            return !(lhs == rhs);
        }

    };


    // aml::entry< K, V > is a key-value-pair, defined in dictionary.hpp.
    // (Objects of this types are not needed)
    //
    // record<  entry< K1, V1 >,  entry< K2, V2>,  ... > is a  metaprogrammable
    // verion of a C-struct.
    //
    // By metaproprogrammable we mean that the parameters K1, K2..., V1, V2... ,
    // which spacify the names and the types of elements can be modified and queried
    // in a metaprogramming context.
    //
    //
    // It is guaranteed that a record r of tye type
    // record<  entry< K1, V1 >,  entry< K2, V2>,  ... > has the same memory-layout as
    // a C-struct
    //                struct r_ { V1 K1_; V2 K2_; .... }.
    //
    // Here K1_ is a variable name, which is used to refer to a memory location in r_,
    // whereas K1 is a type that corresponds to the corresponding memory location in r.
    // The same olds for K2_ versus K2 etc...
    //
    template<  typename... Entry  >
    struct record
    :    private indexed_type< typename Entry::key, typename Entry::value>...
    {
    private:
        using lookup_table  =  dictionary<Entry...>;

        // The assertion enforces an error if two entries have the same
        // key, as then lookup_table will not exist, and hence the call of
        // lookup_table::size() wil result in a compilation error.
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


        constexpr record()                          =  default;

        constexpr record( record const& )           =  default;

        constexpr record( record && other)          =  default;

        template< typename... Entry_ >
        constexpr record( record<Entry_...> const& other )
        // noexcept specification missing
        :    subtype<Entry> (other.template cref< typename Entry::key >() )...
        { }

        template< typename... Entry_ >
        constexpr record( record<Entry_...>&& other )
        // noexcept specification missing
        :    subtype<Entry> (other.template rref< typename Entry::key >() )...
        { }


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
        constexpr auto const& ref() const  noexcept
        {
            return cref<Key>();
        }


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


        // Take const references to all items in ascending order as
        // function arguments and evaluate
        template< typename F >
        auto const_invoke(F&& f) const
            noexcept( noexcept(f(cref<typename Entry::key>()... ) ) )
        {
            return f( cref<typename Entry::key>()... );
        }


        // Take rvalue references to all items in ascending order as
        // function arguments and evaluate
        template< typename F >
        auto move_invoke(F&& f)
            noexcept( noexcept(f(rref<typename Entry::key>()... ) ) )
        {
            return f( rref<typename Entry::key>()... );
            //return  f( static_cast< subtype<Entry>& >(*this).rref()... );
        }


        friend constexpr bool operator==( record<Entry...> const& lhs,
                                          record<Entry...> const& rhs )
        {
            return  ( (  static_cast< indexed_type< typename Entry::key, typename Entry::value> const&>(lhs)
                         ==
                         static_cast< indexed_type< typename Entry::key, typename Entry::value> const&>(rhs)
                      ) && ... );
            //            return ( (lhs.cref<typename Entry::key>() == rhs.cref<typename Entry::key>()) && ... );
        }


        friend constexpr bool operator!=( record<Entry...> const& lhs,
                                          record<Entry...> const& rhs )
        {
            return ! (lhs == rhs);
            //            return ( (lhs.cref<typename Entry::key>() != rhs.cref<typename Entry::key>()) || ...  );
        }
    };
}
