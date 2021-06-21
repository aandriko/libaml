#pragma once

namespace aml
{
    template<  template< typename... > class F  >
    struct function
    {
        template< typename... X >
        using apply_to  =  F< X... >;
    };


    template< typename... Subterms >
    struct subterms;


    template<>
    struct subterms<>
    {
        template<  template< typename... > class F  >
        using apply = F< >;

        static auto constexpr size() { return 0; }
    };


    template< typename    H
            , typename... T >
    struct subterms< H, T... > 
    {
        using head  =  H;
        using tail  =  subterms< T... >;

        template<  template< typename... > class F  >
        using apply = F< H, T... >;

        static auto constexpr size() { return 0; }
    };


    template< typename    T
            , typename... no_args
            >
    struct term
    {
    private:
        static_assert(sizeof...(no_args) == 0, "term<> takes only one argument!");

        template<typename Y, typename... no_args_>
        struct identity_
        {
            static_assert(sizeof...(no_args_) == 0,
                          "The identity function takes exactly one argument. ");

            using type  =  Y;
        };

    public:

        template<typename... Y>
        using function  =  typename identity_<Y...>::type;

        using subterms  = aml::subterms<T>;
    };


    template<  template< typename...> class F
            ,  typename... X
            >
    struct term<  F< X... >  >
    {
        template<typename... Y>
        using function  =  F< Y... >;

        using subterms  =  aml::subterms< X... >;
    };

    // compile-time test if two types (X, Y) are equal/unequal:
    // type_id<X> == type_id<Y> / type_id<X> != type_id<Y>
    template< typename T >
    inline void term_id() {};

}