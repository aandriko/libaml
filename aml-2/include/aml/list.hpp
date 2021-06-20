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
    struct drop< 0, List >
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


    template<typename... X>
    struct size
    {
        static constexpr auto eval() { return sizeof...(X); };
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
        using map_accum_left_with  =  list<b, conslist<> >;


        template< template< tyepname...> class
                , typename b
                >
        uisng map_accum_right_with  = list<b, conslist<> >;


        template<int n>
        using drop  =  apply<aml::drop<n>::template apply_to>;


        template<int n>
        using take  =  apply<aml::take<n>::template apply_to>;


        template<template<typename...> class>
        using split_by_first_occurence_of  =  list< list<>, list<> >;


        template<template<typename...> class>
        struct partition_with : public list< list<>, list<> >
        {
            using accepted  =  list<>;
            using rejected  =  list<>;
        };


        template<template<typename...> class >
        using sort_with  =  list<>;


        template< template<typename...> class >
        using take_while = list<>;


        template< template<typename...> class >
        using drop_while = list<>;


        static constexpr auto size() { return 0; }


        template<typename... Y>
        friend auto operator+(list<Y...>)  ->  list<Y...>;

    private:
        template<typename...>
        friend struct list;


        template<template<typename...> class>
        struct sort_
        {
            using tyep = list<>;
        };
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


    public:
        // (b -> a -> (b, c) ) -> b -> [a] -> (b, [c])
        template< template< typename...> class F // (b-> a -> (b, c)
                , typename b
                >
        using map_accum_left_with  =  list<  typename F< typename init::
                                                         template map_accum_left_with< F, b >::head,   last >::head,

                                             typename init::template map_accum_left_with<F, b>::
                                             tail::head::
                                             template rcons<  typename F<  typename init::
                                                                           template map_accum_left_with<F, b>::head,

                                                                           last
                                                                        >::tail::head  >
                                          >;


        template< template< typename...> class F // (b-> a -> (b, c)
                , typename b
                >
        using map_accum_right_with  =  list<  typename F< typename tail::
                                                          template map_accum_right_with< F, b >::head,   last >::head,

                                              typename tail::template map_accum_left<F, b>::
                                              tail::head::
                                              template cons<  typename F<  typename tail::
                                                                           template map_accum_right_with<F, b>::head,

                                                                           last
                                                                         >::tail::head  >
                                           >;


        template<int n>
        using drop  =  apply<aml::drop<n>::template apply_to>;


        template<int n>
        using take  =  apply<aml::take<n>::template apply_to>;


        template<int... n>
        using at  =  list< typename drop<n>::head... >


        static auto constexpr size() { return 1 + sizeof...(T); }


        template<typename... Y>
        friend auto operator+(list<Y...>)  ->  list<H, T..., Y...>;


    private:
        template<template<typename...> class Pred>
        struct split_
        {
            struct after_head
            {
                using type_  =  typename tail::template split_by_forst_occurrence_of<Pred>;
                using type   =  list<typename type_::head::template rcons< H >, typename type_::tail>;
            };

            struct at_head
            {
                using type  =  list< list<>, list<H, T...> >;
            };
        };

    public:
        template<template<typename...> class Pred>
        using split_by_first_occurence_of  =  typename drop< static_cast<bool>(Pred<H>::eval()),
                                                             list< typename split_<P>::after_head, typename split_<P>::at_head > >::
                                               head::type;


    private:
        template<template<typename...> class Pred>
        struct partition_tail_
        {
            using partition_ = typename tail::template partition_with<Pred>;
            using add_head_to_accepted_ =  list< typename partition_::accepted::template rcons<head>,
                                                 partition_::rejected >;

            using add_head_to_rejected_  =  list< typename partition_::accepted,
                                                 typename partition_::rejected::template rcons<head> >;

            using add_H_ = typename drop <    static_cast<bool>(Pred<H>::eval()),
                                              add_head_to_rejected_,
                                              add_head_to_accepted_    >::head;
        };

    public:

        template<template<typename...> class Pred>
        struct partition_with
        :
            public typename parition_tail_<Pred>::template add_head
        {
            using accepted = typename partition_tail_<Pred>::add_H_::head;
            using rejected = typename partition_tail_<Pred>::add_H_::tail::head;
        };


    private:
        tmeplate<template<typename...> class Less>
        struct sort_
        {
            template<typename X>
            using P = Pred<X, H>;

            using lower  =  typename tail::partition_with<P>::accepted::template sort_< Less >::type;
            using upper  =  typename tail::partition_with<P>::rejected::template sort_< Less >::type;

            using type  =  typename upper::template apply<typename lower::template rcons<H>::template rcons>;
        };


        template<typename List, typename X>
        using add_indexed_parameter  =  typename List::template rcons< list< X, size< List > > >;


        using indexed_list  =  typename list< H, T... >::
                               template lfold_with<add_indexed_parameter, list<> >;


        template< template<typename...> class Less >
        struct indexed_less
        {
            template<typename X, typename Y>
            struct apply_to
            {
                static constexpr eval()
                {
                    return    Less<X, Y>::eval()    ||
                              ( ! Less<Y, X>::eval()  &&  X::tail::head::eval() < Y::tail::head::eval() );
                }
            };
        };

        template<typename X>
        using remove_index = typename X::head;

    public:

        template<template<typename...> class Less>
        using sort_with  =  typename indexed_list::template sort_< indexed_less<Less> >::type::template pointwise_apply<remove_index>;


    private:

        template< template<typename...> class Predicate
                , typename Prefix
                >
        struct take_while_
        {
            struct add_head
            {
                using type  =  typename tail::template take_while<Predicate, typename Prefix::rcons<H>>;
            };

            struct stop
            {
                using type  =  Prefix;
            };

            using type  = typename take< static_cast<bool>(Predicate<H>::eval()), stop, add_head >::type;
        };


        template< template<typename...> class Predicate >
        struct drop_while_
        {
            struct drop_head
            {
                using type = typename tail::template drop_while<Predicate>;
            };

            struct stop
            {
                using type = list<H, T...>;
            };

            using type = typename take< Predicate<H>::eval()
            
            
        }

    public:

        template< template<typename...> class Predicate
                , typename Prefix = list<>
                >
        using take_while  = typename take_while_<Predicate, Prefix>::type;


        template<template<typename...> class Predicate>
        using drop_while;

    };



    //    template<auto... idx>
    //    struct lambda
    //    {
    //        template<typename... X>
    //        using apply_to = typename list<X...>::template at<idx...>;
    //    };


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


    template<typename... X>
    using split_by_first_occurrence  =  typename tail<X...>::template split_by_first_occurrence_of< head<X...>::template apply_to >;


    template<typename... X>
    using partition  =  typename tail<X...>::template partition_with< head<X...>::template apply_to >;


    template<typename... X>
    using sort  =  typename tail<X...>::template sort_with< head<X...>::template apply_to >;
}
