#pragma once


#include "../basic_types.hpp"


namespace aml::structure
{
    template<typename...>
    struct term;


    template<typename Linker, template<typename...> class F, typename... X>
    struct term<Linker, F<X...> >
    {
        template<typename... Y>
        using function = F<Y...>;

        using subterms = typename conslist<X...>::template apply<Linker::template abstract_type>;
    };


    template<typename Linker, typename Atomic_Term>
    struct term<Linker, Atomic_Term>
    {
        template<typename... X>
        using function = aml::identity<X...>;

        using subterms = typename conslist<Atomic_Term>::template apply<Linker::template abstract_type>;
    };


}
