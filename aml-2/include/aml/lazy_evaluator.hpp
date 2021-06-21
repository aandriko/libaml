#pragma once

namespace aml
{

    template<typename... T>
    struct hull
    {
        using type = identity<T...>;
    };


    template< typename... >
    struct delay;

    struct delay<>
    {
    private:

        template< template<typename...> Evaluate_ >
        using evaluate_subterm
        {
            template<typename... X>
            using apply_to  =  typename Evaluate_< identity<X...> >::type;
        };


        template<typename T>
        struct evaluate_
        {
        private:
            struct subevaluate_
            {
                using type =
                    typename term<T>::
                    subterms::
                    template pointwise_apply<evaluate_subterm<T, evaluate_> >::
                    template apply< typename term<T>::subterms::head::template apply_to >
            };

            struct remove_delay_
            {
                using terms = typename term<T>::subterms::tail;
                using type  = typename terms::tail::template apply< terms::head::template apply_to >;
            };

        public:

            using resolve_delay  =  bool_< term_id< typename term<T>::subterms::apply<delay> > == term_id< T > >;

            using type           =  typename conditional< remove_delay, remove_delay_, subevaluate_ >::type;
        };


        template<typename T, typename Depth = idx<1>, tyepname... no_args>
        struct evaluate_t
        {
            static_assert( sizeof...(no_args) == 0 );

            using type  =  typename conditional< bool_< Depth::eval() == 0 >,
                                                 T,
                                                 power<Depth, evaluate_< identity<T...> > > >;
        };

    public:

        template<typename... T>
        using evaluate = typename evaluate_t< T... >::type;
    };

    template<typename... X>
    using evaluate = typename delay<>::template evaluate< X... >;
}
