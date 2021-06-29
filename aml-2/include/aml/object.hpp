#pragma once

namespace aml
{
    template< typename... >
    struct list;


    template<auto... x>
    struct object
    {
        static_assert( sizeof...(x) <= 1 );
    };


    template<auto x>
    struct object<x>
    {
        static constexpr auto eval() { return x; }
    };


    template<>
    struct object<>
    {
        template<auto... x>
        struct list
        {
            using as_types = aml::list< object<x>... >;

            template<  template<auto... >  class F  >
            using apply  =  F< x... >;
        };


        template< typename... X >
        using from_types  =  list< X::eval()... >;
    };

    template< auto n >
    struct num
    {
        using up   = num< n + 1 >;
        using down = num< n - 1 >;

        static constexpr auto eval() { return n; }
    };


    constexpr auto infinity = nullptr;

    using infinity_t  =  num< infinity >;

    template<>
    struct num< infinity >
    {
        using up    =  infinity_t;
        using down  =  infinity_t;

        static constexpr auto eval() { return infinity; }
    };


}
