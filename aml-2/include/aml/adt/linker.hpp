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
    using subtype  =  aml::entry<Symbol, aml::function<SubType> >;


    template<typename... Indexed_Type>
    auto make_record( Indexed_Type&&... field)
    {
        using record_t  =  record<  typename term<std::decay_t<Indexed_Type>>::
                                    subterms::
                                    template apply< entry >...  >;

        return record_t( field.rref()... );
    }


    template<  typename    Symbol
            ,  typename... Args >
    auto type(Args&&... args)
    {
        using return_t  =  indexed_type<  Symbol, aml::adt::record< aml::entry< Args&&, Args&& >... >  >;
        return return_t(static_cast<Args&&>(args)...);
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
        :
            public SubType::value::template apply_to< X... >...
        {
        private:
            using this_type  =  adt< X... >;


            template< typename SubType_ >
            using symbol  =  typename SubType_::key;

            template< typename SubType_ >
            using instance  =  typename SubType_::value::template apply_to<X...>;


            using data_t  =  record<  aml::entry<  symbol< SubType >
                                                ,  instance< SubType >  >...  >;


            data_t data_;


            struct internal_ {};
            struct internal_unpermuted_{};

            //            template<typename... Args>
            //  adt(internal_inpermuted, Args&&...
            
            template<typename Record>
            adt( internal_, Record r)
            :   instance<SubType>(    r.template rref<symbol<SubType> >() // indexed_type
                                       .move_invoke(  [](auto&&... x)
                                                      {
                                                          using instance_t = instance<SubType>;

                                                          auto ret_val  =   instance_t( static_cast<decltype(x)&&>(x)... );

                                                          return ret_val;
                                                      }  )    )...
                                      //(   [](auto&&... z ) {  return instance<SubType>(static_cast< decltype(z)&& >(z)... ); }  ) )...
            { }

            template<typename T>
            using make_entry =  typename aml::term<std::decay_t<T> >::subterms::template apply<aml::entry>;

        public:
            using linker  =  link< SubType... >;


            template<  typename... Args
                    ,  typename  =  typename  // exclude copy and move construction
                                    none<  is_same<  list< Args... >, list< this_type const & >  >
                                        ,  is_same<  list< Args... >, list< this_type & >  >
                                        ,  is_same<  list< Args... >, list< this_type && > >
                                        >::sfinae
                >
            adt( Args&&... args )
            :    adt( internal_{},  make_record( static_cast<Args&&>(args)... ) )
            { }

            adt()              =  default;

            adt( adt const& )  =  default;
            adt( adt &&     )  =  default;


            template<typename S>
            auto const&  operator[](const S&) const&
            {
             return data_.template cref< std::decay_t<S> >();
             //                return static_cast< instance<std::decay_t<S> > const& >(*this);
            }


            template<typename S>
            auto& operator[](const S&) &
            {
             return data_.template ref< std::decay_t<S> >();
             //                return static_cast< instance<std::decay_t<S> >& >(*this);
            }


            template< typename S >
            auto&& operator[](S const&) &&
            {
             return data_.template rref< std::decay_t<S> >();
             //                return static_cast< instance< std::decay_t<S> >&& >(*this);
            }

        };


    };

}
