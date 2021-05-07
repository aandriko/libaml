#pragma once

#include "syntax.hpp"
#include "adt/linker.hpp"


namespace aml::adt
{       
    template<typename Linker, typename...>
    struct syntax_t
    {
        template<typename Term>
        struct term
        {
            template<typename... X>
            using function = typename aml::syntax::term<Term>::template function<X...>;

            using subterms = typename aml::syntax::term<Term>::subterms::template apply<Linker::template list_type>;
        };
    };

    
    template<typename... Linker>
    using syntax = syntax_t<Linker...>;            
}
