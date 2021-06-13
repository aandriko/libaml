#pragma once


namespace aml
{
    template<typename...>
    struct list;


    template< template< typname... > class List
            , int... n
            >
    struct multi_index
    {
        using as_types = list< multi_index<List, n>... >;
        eval(); ...
    };


    template< int n
            , template<typename...> class List = list
            >
    struct drop
    {
        template< typename... X >
        using from  =  typename List< X... >::
                       tail::
                       template apply< typename drop< n-1 >::template from >;
    };


    template< template<typename...> class List >
    struct drop< 0 >
    {
        template<typename... X>
        using from = list< X... >;
    };


    template< int n
            , template<typename...> class List = list
            >
    struct take
    {
        template< typename... X >
        using from  =  typename List::
                       template reverse< X... >::
                       template apply< drop< sizeof...(X) - n >::template from >::
                       reverse;
    };


    template<>
    struct list<>
    {
    public:

        template<typename... X>
        using cons  =  typename list<X...>::reverse;


        template<typename... X>
        using rcons  =  list<X...>;


        using reverse  =  list<>;


        template< template< typename... > class F >
        using apply  =  F<>;


        template< template< typename... > class >
        using pointwise_apply  =  list<>;


        template< template< typename... > class
                , typename Z
                >
        using rfold_with = Z;


        template< template<typename...> class
                , typename Z
                >
        using lfold_with = Z;


        template<int n>
        using drop  =  apply<aml::drop<n>::template from>;


        template<int n>
        using take  =  apply<aml::take<n>::template from>;


        static constexpr auto size() { return 0; }


        template<typename... Y>
        friend auto operator+(list<Y...>)  ->  list<Y...>;
    };


    template<typename H, typename... T>
    struct list<H, T...>
    {
        template<typename... X>
        using cons  =  list<X..., H, T...>;


        template<typename... X>
        using rcons  =  list<H, T..., X...>;


        using head  =  H;
        using tail  =  list<T...>;


        using reverse  =  typename tail::reverse::template rcons<H>;


        template<template<typename...> class F>
        using apply  =  F<H, T...>;


        template<template<typename...> class F>
        using pointwise_apply  =  list< F< H >,  F< T >... >;


        template< template<typename...> class F
                , typename Z
                >
        using rfold_with =  F<H, typename list<T...>::template rfold_with<F, Z> >;


        template< template<typename...> class F
                , typename Z
                >
        using lfold_with  =  F< typename reverse::tail::reverse::template lfold_with<F, Z>,

                                typename reverse::head>;


        template<int n>
        using drop  =  apply<aml::drop<n>::template from>;


        template<int n>
        using take  =  apply<aml::take<n>::template from>;


        static auto constexpr size() { return 1 + sizeof...(T); }


        template<typename... Y>
        friend auto operator+(list<Y...>)  ->  list<H, T..., Y...>;
    };


    template<typename... X>
    using reverse = typename conslist<X...>::reverse;


    template< typename... List >
    using join  =  decltype( (List() + ... ) );


    template<typename... X>
    using head = typename list<X...>::head;


    template<typename... X>
    using tail = typename list<X...>::tail;


    template<typename... X>
    struct size
    {
        static constexpr auto eval() { return sizeof...(X); };
    };


    template<typename... X>
    using cons = decltype(typename tail<X...>::reverse() + head<X...>());


    template<typename... X>
    using rcons = decltype( head<X...>() + tail<X...>() );


    template<int n, typename... X>
    using take_elements = typename take<n>::template from<X...>;


    template<int n, typename... X>
    using drop_elements = typename drop<n>::template from<X...>;


    template<typename... List>
    using join  =  decltype( List() + ... );


    template<typename... List>
    using join_and_revert =  typename join<List...>::reverse;


    // lfold<F, Z, X1, X2, X3... XN> = F<Z, F< X1, F< X2, .... , F<Xn> > ....> >
    template<typename... X>
    using lfold  = typename tail<tail<X...> >::
                   template lfold_with<    head<X...>::template apply,
                                           head<tail<X...> >    >;


    // lfold<F, Z, X1, X2, X3... XN> =
    //                      F<X1, ... F<X{n-2}, F< X{N-1}, F<XN, Z>>> ... >
    template<typename... X>
    using rfold  = typename tail<tail<X...> >::
                   template lfold_with<    head<X...>::template apply,
                                           head<tail<X...> >    >;


    template<typename... X>
    using pointwise_apply  =  typename tail<X...>::
                              template pointwise_apply< head<X...>::
                                                        template apply_to >;


    // intersperse<Z, X1, X2, X3 ... > = list<X1, Z, X2, Z, X3, Z ... >
    template<typename... X>
    using intersperse  =  typename list< list< X, head< X...> >... >::tail::
                          template apply<join>;

}
