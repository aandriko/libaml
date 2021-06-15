#pragma once


namespace aml
{
    template< typename... >
    struct list;


    template< int n
            , template<typename...> class List = list
            >
    struct drop
    {
        template< typename... X >
        using apply_to  =  typename List< X... >::
                           tail::
                           template apply< typename drop< n-1 >::template apply_to >;
    };


    template< template<typename...> class List >
    struct drop< 0 >
    {
        template<typename... X>
        using from  =  List< X... >;
    };


    template< int n
            , template<typename...> class List = list
            >
    struct take
    {
        template< typename... X >
        using apply_to  =  typename List::
                           template reverse< X... >::
                           template apply< drop< sizeof...(X) - n >::template apply_to >::
                           reverse;
    };


    template<>
    struct list<>
    {
    public:

        template<typename... X>
        using cons  =  typename list<X...>;


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
        using rscan_with  =  Z;


        template< template< typename... > class
                , typename Z
                >
        using lscan_with  =  Z


        template< template< typename... > class
                , typename Z
                >
        using rfold_with = Z;


        template< template<typename...> class
                , typename Z
                >
        using lfold_with = Z;


        template< template< typename...> class
                , typename b
                >
        using map_accum_left  =  conslist<b, conslist<> >;


        template< template< tyepname...> class
                , typename b >
        uisng map_accum_right  = conslist<b, conslist<> >;


        template<int n>
        using drop  =  apply<aml::drop<n>::template apply_to>;


        template<int n>
        using take  =  apply<aml::take<n>::template apply_to>;


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


        using last     =  typename reverse::head;
        using init     =  typename reverse::tail::reverse;


        template<template<typename...> class F>
        using apply  =  F<H, T...>;


        template<template<typename...> class F>
        using pointwise_apply  =  list< F< H >,  F< T >... >;


        template< template< typename... > class F
                , typename Z
                >
        using rfold_with =  F< head, typename tail::template rfold_with<F, Z> >;


        template< template<typename...> class F
                , typename Z
                >
        using lfold_with  =  F< typename init::template lfold_with<F, Z>, last >;


        template< template< typename... > class F
                , typename Z
                , typename Already_Scanned  =  typename tail::
                                               template rscan_with< F, Z >
                >
        using rscan_with  = typename Already_Scanned::
                            head::
                            template cons< F< head, typename Already_Scanned::head > >;


        template< template< typename... > class F
                , typename Z
                , typename Already_Scanned  =  typename tail::
                                               template lscan_with<F, Z>
                >
        using lscan_with  =  typename Already_Scanned::
                             last::
                             template rcons< F< typename Already_Scanned::last, last > >;

        // (b -> a -> (b, c) ) -> b -> [a] -> (b, [c])
        template< template< typename...> class F // (b-> a -> (b, c)
                , typename b
                >
        using map_accum_left  = tail::template apply<map_accum_left, b>

            conslist<b, conslist<> >;


        template< template< tyepname...> class
                , typename b >
        uisng map_accum_right  = conslist<b, conslist<> >;


        template<int n>
        using drop  =  apply<aml::drop<n>::template apply_to>;


        template<int n>
        using take  =  apply<aml::take<n>::template apply_to>;


        template<int... n>
        using at  =  list< typename drop<n>::head... >


        static auto constexpr size() { return 1 + sizeof...(T); }


        template<typename... Y>
        friend auto operator+(list<Y...>)  ->  list<H, T..., Y...>;
    };


    template<typename... X>
    struct size
    {
        static constexpr auto eval() { return sizeof...(X); };
    };


    template<typename... X>
    using head = typename list<X...>::head;


    template<typename... X>
    using tail = typename list<X...>::tail;


    template<typename... X>
    using reverse  =  typename list<X...>::reverse;


    template<typename... X>
    using init  =  typename list<X...>::init;


    template<typename... X>
    using last  =  typename list<X...>::last;


    template< typename... List >
    using join  =  decltype( (List() + ... ) );


    template<typename... X>
    using cons = decltype(typename tail<X...>::reverse() + head<X...>());


    template<typename... X>
    using rcons = decltype( head<X...>() + tail<X...>() );


    template<typename... List>
    using join  =  decltype( List() + ... );


    template<typename... X>
    using reverse = typename conslist<X...>::reverse;


    // lfold<F, Z, X1, X2, X3... XN> = F<Z, F< X1, F< X2, .... , F<Xn> > ....> >
    template<typename... X>
    using lfold  = typename drop<2>::template apply_to<X...>::
                   template lfold_with
                   <
                       typename take<0>::template apply_to<X...>::template apply_to,
                       typename take<1>::template apply_to<X...>
                   >;



    // lfold<F, Z, X1, X2, X3... XN> =
    //                      F<X1, ... F<X{n-2}, F< X{N-1}, F<XN, Z>>> ... >
    template<typename... X>
    using rfold  = typename drop<2>::template apply_to<X...>::
                   template rfold_with
                   <
                       typename take<0>::template apply_to<X...>::template apply_to,
                       typename take<1>::template apply_to<X...>
                   >;


    // intersperse<Z, X1, X2, X3 ... > = list<X1, Z, X2, Z, X3, Z ... >
    template<typename... X>
    using intersperse  =  typename list< list< X, head< X... > >... >::tail::
                          template apply<join>;


    template<typename... X>
    using map  =  typename tail<X...>::
                  template pointwise_apply< head<X...>::template apply_to >;



}
