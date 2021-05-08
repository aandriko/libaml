#pragma once

#include "syntax.hpp"
#include "adt/linker.hpp"


namespace aml::adt
{
    template<typename...>
    struct term_t;
    
    template<typename Linker, typename Term>
    struct term_t<Linker, Term>
    {
        using term = term_t<Linker, Term>;
        
        //        template<typename... X>
        //        using function = typename aml::syntax::term<Term>::template function<X...>;
        
        //        using subterms = typename aml::syntax::term<Term>::subterms::template apply<Linker::template list>;
        template<typename... X>
        using function = typename aml::syntax::term<Term>::template function<X...>;

        //        using subterms = typename aml::syntax::term<Term>::subterms::template apply<Linker::template abstract_type>::template term_t<Linker, Term>;

        using subterms =
            typename aml::syntax::term<Term>::subterms
            ::template apply
            <
                Linker::template subtype< aml::adt::trivially_linked< ::list> >::template t
            >;

        //                    Linker::template abstract_type>::template term_t<Linker, Term>;
        
    };
    
    
    template<typename... Linker_and_Term>
    using term = term_t<Linker_and_Term...>;            
}
