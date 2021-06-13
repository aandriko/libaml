#pragma once

namespace aml
{
    template<typename...>
    struct is_same;


    template<>
    struct is_same<>
    {
        static constexpr bool eval() { return true; }
    };


    template< typename A >
    struct is_same<>
    {
        static constexpr bool eval() { return true; }
    };


    template< typename A
            , typename B
            >
    struct is_same<A, B>
    {
        static constexpr bool eval() { return false; }
    };


    template< typename A >
    struct is_same<A, A>
    {
        static constexpr bool eval() { return true; }
    };


    template< typename    A
            , typename    B
            , typename... C
            >
    struct is_same<A, B, C...>
    {
        static constexpr bool eval()
        {
            return eval<is_same<A, B>>()  &&  eval<is_same<B, C...>>();
        };
    };


}
