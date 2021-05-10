#pragma once

#include "../exponents.hpp"
#include "../string.hpp"

namespace aml::adt
{
    template<typename...>
    struct term;

    
    template<typename Linker, template<typename...> class F, typename... X>
    struct term<Linker, F<X...> >
    {
        template<typename... Y>
        using function = F<Y...>;

        using subterms = typename Linker::template subtype< decltype("list"_), X...>;
    };


    template<typename Linker, typename Atomic_Term>
    struct term<Linker, Atomic_Term>
    {
        template<typename... X>
        using function = aml::identity<X...>;

        using subterms = typename Linker::template subtype<decltype("list"_), Atomic_Term>;
    };
    

}
