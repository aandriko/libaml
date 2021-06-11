#pragma once

#include "./conslist.hpp"

namespace aml
{
    template<typename... T>
    struct enable
    {
    private:
        static_assert(sizeof...(T) == 1, "");

        template<bool b>
        struct if_aux_
        {
            static_assert(b, "Enablement condition broken.");
            using type = typename conslist<T...>::head;
        };

    public:
        template<typename... Bool>
        using if_ = typename if_aux_< (Bool::eval() && ...) >::type;
    };


    template<typename... Bool>
    struct if_
    {
        template<typename... T>
        using enable = typename aml::enable<T...>::template if_<Bool...>;

    private:
        static_assert( (Bool::eval() || ... || true), "Arguments for eval must be of a boolean type.");

    };



    template<bool>
    struct bool_;


    using true_  = bool_<true>;
    using false_ = bool_<false>;


    template<typename...> using top    = true_;
    template<typename...> using bottom = false_;


    template<typename... X>
    using all = bool_< (X::eval() && ... ) >;


    template<typename... X>
    using one = bool_< (X::eval() || ... ) >;


    template<typename... X>
    using none = bool_< ! all<X...>::eval() >;


    template<template<typename...> class... Pred>
    struct for_all
    {
        template<typename... X>
        using apply_to = bool_<(Pred<X...>::eval() && ... )>;
    };


    template<template<typename...> class... Pred>
    struct for_none
    {
        template<typename... X>
        using apply_to = bool_<(!Pred<X...>::eval() && ... )>;
    };


    template<template<typename...> class... Pred>
    struct for_one
    {
        template<typename... X>
        using apply_to = bool_<(Pred<X...>::eval() || ... )>;
    };


    template<>
    struct bool_<true>
    {
        static constexpr bool eval() { return true; }

        using sfinae = true_;

        template<typename... Alternatives>
        using conditional = typename
            enable< typename conslist<Alternatives...>::head >::
            template if_<bool_<sizeof...(Alternatives) == 2>>;
    };


    template<>
    struct bool_<false>
    {
        static constexpr bool eval() { return false; }


        template<typename... Alternatives>
        using conditional = typename
            enable< typename conslist<Alternatives...>::tail::head >::
            template if_<bool_<sizeof...(Alternatives) == 2> >;
    };


    template<typename... CTF>
    using conditional = typename

        conslist<CTF...>::tail::

        template apply
        <
            bool_< conslist<CTF...>::head::eval()>::template conditional
        >;


    template<typename... instantiation_error>
    struct is_same
    {
        static_assert(sizeof...(instantiation_error) == 2,
                      "Error: is_same has not been instantiated with two arguments.");
    };


    template<typename X, typename Y>
    struct is_same<X, Y> : public false_ {};


    template<typename X>
    struct is_same<X, X> : public true_ {};


    template<typename T>
    using add_type = typename T::type;


    template<typename... X>
    using identity = typename enable< typename conslist<X...>::head >::template if_<bool_<sizeof...(X) == 1> >;


    template<typename...>
    struct term;


    template<template<typename...> class F, typename... X>
    struct term<F<X...>>
    {
        template<typename... Y>
        using function = F<Y...>;

        using subterms = conslist<X...>;
    };


    template<typename Atomic_Term>
    struct term<Atomic_Term>
    {
        template<typename... X>
        using function = aml::identity<X...>;

        using subterms = conslist<Atomic_Term>;
    };


    template<typename... T>
    struct hull;


    template<typename T>
    struct hull<T>
    {
        using type = T;
    };


    template<template<typename...> class... >
    struct function;


    template<template<typename...> class F>
    struct function<F>
    {
        template<typename... X>
        using apply_to = F<X...>;
    };


    template<typename... Conslist>
    struct ket
    {
        static_assert(sizeof...(Conslist) == 1, "The parameter-list of ket must consist of a single conslist.");
        static_assert(aml::is_same
                      <
                          typename term<typename conslist<Conslist...>::head>::function,
                          aml::function<conslist>
                      >::eval(),
                      "The parameter of ket must be a conslist");
    };


    template<typename... X>
    struct ket<conslist<X...> >
    {
        template<template<typename...> class F>
        using bra = F<X...>;
    };


    template<template<typename...> class F>
    struct bra
    {
        template<typename... Conslist>
        using ket = typename aml::ket<Conslist...>::template bra<F>;
    };


    template<auto... o>
    struct object;


    template<auto o>
    struct object<o>
    {
        static constexpr auto eval() { return o; }
    };


    template<>
    struct object<>
    {
        template<auto... obj>
        struct list
        {
            template<template<auto...> class F>
            using apply = F<obj...>;
        };
    };


    template<typename...>
    struct entry;


    template<typename Key, typename Value>
    struct entry<Key, Value>
    {
        using key   = Key;
        using value = Value;

        static constexpr auto eval() { return Key::eval(); }
    };


    template<typename... X>
    using key = typename conslist<X...>::template check<sizeof...(X) == 1>::head::key;


    template<typename... X>
    using value = typename conslist<X...>::template check<sizeof...(X) == 1>::head::value;


    template<template<typename...> class... >
    struct composition;


    template<>
    struct composition<>
    {
        template<typename... X>
        using apply_to = identity<X...>;
    };


    template<template<typename...> class F>
    struct composition<F>
    {
        template<typename... X>
        using apply_to = F<X...>;
    };


    template< template<typename...> class    G
            , template<typename...> class... F
            >
    struct composition<G, F...>
    {
        template<typename... X>
        using apply_to = G< typename composition<F...>::template apply_to<X...> >;
    };


    template< template<typename...> class... F >
    struct pointwise_action
    {
        template<typename... X>
        using apply_to = conslist< F<X>... >;
    };

}
