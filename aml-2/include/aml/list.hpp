#pragma once

#include "./functional.hpp"
#include "./logic.hpp"
#include "./object.hpp"

namespace aml
{
    template< typename... X >
    struct head_and_tail;

    template<>
    struct head_and_tail<>
    {
        struct tail
        {
            using type     =  tail;
            using return_  =  head_and_tail<>;
        };

        template<  template<typename...> class F  >
        using apply = F<>;
    };


    template< typename H, typename... T>
    struct head_and_tail<H, T...>
    {
        using head = H;


        struct tail
        {
            using type     =  typename head_and_tail<T...>::tail;
            using return_  =  head_and_tail< H, T... >;
        };


        template<  template<typename...> class F  >
        using apply  = F< H, T... >;
    };


    template<typename... X>
    using head = typename head_and_tail<X...>::head;


    template< typename n, typename... X  >
    using at  =  typename power< typename head_and_tail<X...>::tail, n >::return_::template apply<head>;


    template< typename    List
            , typename... no_args
            >
    using list_head  = typename identity<List, no_args...>::head;


    template< typename    List
            , typename... no_args
            >
    using list_tail  =  typename identity<List, no_args...>::tail;


    template< typename List, typename... no_args >
    using list_last  =  typename identity<List, no_args...>::last;


    template< typename List
            , typename... no_args
            >
    using list_init  =  typename identity< List, no_args... >::init;


    template< typename... >
    struct list;


    template<>
    struct list<>
    {
        using head  =  list<>;  // useful definitions for monoid<list_head> and monoid<list_tail>
        using tail  =  list<>;  // (meta-ducktyping).

        static constexpr auto size() { return 0; }

        template< typename... X >
        using cons  =  list< X... >;

        template< typename... X >
        using rcons  =  list< X... >;

        using reverse  =  list<>;


        template< template< typename... > class F >
        using apply  =  F<>;


        template< template< typename... > class >
        using pointwise_apply  =  list<>;


        template< template< typename... > class
                , typename Z
                >
        using rscan_with  =  list<Z>;


        template< template< typename... > class
                , typename Z
                >
        using lscan_with  =  list<Z>;


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
        using map_accum_left_with  =  list<b, list<>>;


        template< template< typename...> class
                , typename b
                >
        using map_accum_right_with  = list<b, list<> >;


        template<typename N>
        using drop  =  typename bool_< N::eval() == 0 >::template enable< list<> >;


        template<typename N>
        using take  =  typename bool_< N::eval() == 0 >::template enable< list<> >;


        template<  template<typename...> class  >
        using drop_while  =  list<>;


        template<  template<typename...> class  >
        using take_while  =  list<>;


        template<template<typename...> class>
        struct partition_with
        {
            using accepted  =  list<>;
            using rejected  =  list<>;

            template<  template< typename... > class F  >
            using apply = F< accepted, rejected >;
        };


        template<template<typename...> class>
        using split_by_first_occurence_of  =  list< list<>, list<> >;


        template<template<typename...> class >
        using sort_with  =  list<>;


        template<typename... X>
        list<X...> operator+(list<X...>);

    private:
        template<typename...>
        friend struct list;

        template<  template< typename... > class  >
        struct sort_with_
        {
            using type = list<>;
        };
    };


    template< typename    H
            , typename... T
            >
    struct list< H, T... >
    {
        static constexpr auto size()  { return 1 + sizeof...(T); }


        template< typename... X >
        using cons  =  list< X..., H, T... >;


        template< typename... X >
        using rcons  =  list< H, T..., X... >;


        using reverse =  typename list< T... >::reverse::
                         template rcons< H >;


        using head  =  H;


        using tail  =  list< T... >;


        using init  =  typename reverse::tail::reverse;


        using last  =  typename reverse::head;


        template<  template< typename... > class F  >
        using apply  =  F< H, T... >;


        template<  template< typename... > class F  >
        using pointwise_apply  =  list< F<H>, F<T>... >;


        template<  template< typename... > class F
                , typename Z
                >
        using rfold_with  =  F< head, typename tail::template rfold_with<F, Z> >;


        template<  template< typename... > class F
                , typename Z
                >
        using lfold_with  =  F< typename init::template lfold_with<F, Z>, last >;


        template<  template< typename... > class F
                ,  typename Z
                >
        using rscan_with  =  typename tail::template rscan_with< F, Z >::
                             template cons< F< head, typename tail::template rscan_with<F, Z>::head > >;


        template<  template< typename... > class F
                ,  typename Z
                >
        using lscan_with  =  typename init::template lscan_with< F, Z >::
                             template rcons<  F< typename init::template lscan_with<F, Z>::last, last >  >;


        // maccL: [ B x A --F--> B x C ] x B --> [A] --> B x [C]
        template<  template< typename...> class F
                ,  typename B
                ,  typename b       =  typename tail::template map_accum_right_with<F, B>::head
                ,  typename c_list  =  typename tail::template map_accum_right_with<F, B>::tail::head
                >
        using map_accum_right_with  =  list< typename F<head, b>::head,
                                             typename c_list::template cons< typename F<head, b>::tail::head > >;


        // maccL: [ B x A --F--> B x C ] x B --> [A] --> B x [C]
        template<  template< typename...> class F
                ,  typename B
                ,  typename b       =  typename init::template map_accum_left_with<F, B>::head
                ,  typename c_list  =  typename init::template map_accum_left_with<F, B>::tail::head
                >
        using map_accum_left_with  =  list< typename F<last, b>::head,
                                            typename c_list::template rcons< typename F<last, b>::tail::head > >;



        template< typename N >
        using drop  =  typename monoid<list_tail>::template power<N>::template apply_to<list<H, T...> >;


        template< typename N >
        using take  =  typename monoid<list_tail>::template power< num<size() - N::eval() > >::template apply_to< reverse >::reverse;


        template<typename... Y>
        list<H, T..., Y...> operator+(list<Y...>);

    private:

        template<typename Pair>
        struct add_to_first
        {
            template< typename X >
            using apply_to  =  list<  typename Pair::head::template rcons<X>,
                                      typename Pair::tail::head  >;
        };

        template< typename Pair >
        struct add_to_second
        {
            template< typename X >
            using apply_to  =  list<  typename Pair::head,
                                      typename Pair::tail::head::
                                      template rcons< X > >;
        };

        template<  template<typename... > class Pred  >
        struct fix_predicate
        {
            template< typename Pair
                    , typename X
                    >
            using add_to_pair  =  typename conditional< bool_<Pred<X>::eval() >,
                                                        add_to_first<Pair>,
                                                        add_to_second<Pair> >::
                                  template apply_to<X>;
        };

    public:

        template<  template< typename... > class Predicate  >
        struct partition_with
        {
        private:
            using partition = rfold_with< fix_predicate<Predicate>::template add_to_pair, list< list<>, list<> > >;

        public:
            using accepted  =  typename partition::head;
            using rejected  =  typename partition::tail::head;

            template<  template< typename... > class F  >
            using apply  =  F< accepted, rejected >;
        };


        template<template<typename...> class Pred>
        struct split_by_first_occurence_of
        {
        private:
            struct ignore_t;

            using  B_start = list< list<>, list<> >; // second list: no occurrence of Pred
                                               // first list: Pred has been satisfied for first member


            template< typename B_times_A
                    , typename B = typename B_times_A::head
                    , typename A = typename B_times_A::tail::head
                    >
            using Pred_  =  bool_<B::head::size() != 0 || Pred<A>::eval()>;


            template<typename B, typename A>
            using F  =  list< typename fix_predicate< Pred_ >::template add_to_pair<B, A>, ignore_t >;

            using partition  =  map_accum_left_with<F, B_start>;

        public:

            using rejected  =  typename partition::tail::head; // rejected items come before(!)
            using accepted  =  typename partition::head;       // accepted items in the parameter list


            template< template<typename...> class G >
            using apply = G< rejected, accepted >;
        };


    private:

        template<typename X, int n>
        struct numbered
        {
            static constexpr int eval() { return n; }
            using type  = X;
        };


        template<typename Numbered>
        using remove_numbering = typename Numbered::type;


        template<typename List, typename X>
        using add_numbered = typename List::template rcons< numbered<X, List::size() > >;

        template<template<typename...> class Less>
        struct stable_less
        {
            template< typename X, typename Y>
            using apply_to = bool_<  Less<X, Y>::eval() || ( Less<X, Y>::eval() == false &&
                                                             Less<Y, X>::eval() == false &&
                                                             X::eval() < Y::eval )  >;
        };

        template<typename...> friend struct list;

        template<  template< typename... > class Less  >
        struct sort_with_
        {
            using pivot = head;

            using partition = partition_with<curry_and_bind< Less, pivot >::template apply_to >;
            // partition::accepted = { y | pivot <  y }
            // partition::rejected = { y | pivot >= y }

            using  upper_part = typename partition::accepted::template sort_with_<Less>::type;
            using  lower_part = typename partition::rejected::template sort_with_<Less>::type;

            using type = typename lower_part::template rcons<pivot>::template apply< upper_part::template cons >;
        };

    public:
        /*
        template< template<typename...> class Less
                >
        using sort_with  =  typename list< H, T... >::
                            template lfold_with< add_numbered, list<> >::
                            template sort_with_<  stable_less< Less >::template apply_to  >::type::
                            template pointwise_apply< remove_numbering >;




        template< template<typename...> class Pred>
        using take_while = typename split_by_first_occurence_of< predicates::none<Pred>::template apply_to >::rejected;


        template< template<typename...> class Pred>
        using drop_while = typename split_by_first_occurence_of< predicates::none<Pred>::template apply_to >::accepted;
        */
    };

    template<typename List, typename... F>
    using list_apply  =  typename List::template apply< identity<F...>::template apply_to >;


    template<typename List, typename... F>
    using list_pointwise_apply  =  typename List::template pointwise_apply< identity<F...>::template apply_to >;


    template<typename... List>
    using join = decltype( ( List() + ... + list<>() ) );


    template< typename List, typename... X>
    using list_cons  =  typename List::template cons<X... >;


    template< typename List, typename... X>
    using list_rcons  =  typename List::template rcons<X... >;


    template< typename... List >
    using list_reverse  =  typename identity<List...>::reverse;


    template< typename List, typename F, typename... Z >
    using list_rscan = typename List::template rscan_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... Z >
    using list_lscan = typename List::template lscan_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... Z >
    using list_rfold  =  typename List::template rfold_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... Z >
    using list_lfold  =  typename List::template lfold_with< F::template apply_to, identity<Z...> >;


    template< typename List, typename F, typename... B>
    using list_map_accum_left  =  typename List::template map_accum_left_with< F::template apply_to, identity<B...> >;

    template< typename List, typename F, typename... B>
    using list_map_accum_right  =  typename List::template map_accum_right_with< F::template apply_to, identity<B...> >;


    template< typename List, typename... N >
    using list_drop  =  typename List::template drop< identity<N...>::eval() >;


    template< typename List, typename... N >
    using list_take  =  typename List::template take< identity<N...>::eval() >;


    template< typename List, typename... Cond >
    using list_drop_while  =  typename List::template drop_while< identity<Cond...>::template apply_to >;


    template< typename List, typename... Cond >
    using list_take_while  =  typename List::template take_while< identity<Cond...>::template apply_to >;


    template< typename List, typename... Less >
    using list_sort  =  typename List::template sort_with< identity<Less...>::template apply_to >;


    template< typename List, typename... Pred >
    using list_partition  =  typename List::template partition_with< identity<Pred...>::template apply_to >;


    template< typename List, typename... Pred >
    using list_split_by_first_occurence  =  typename List::template split_by_first_occurence_of< identity<Pred...>::template apply_to >;

}
