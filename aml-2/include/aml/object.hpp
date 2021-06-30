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

    struct infinity
    {
        using up    =  infinity;
        using down  =  infinity;

        constexpr infinity() = default;
        static constexpr infinity eval() { return infinity(); }
    };

    template<auto x>
    using _ = num<x>;
}
