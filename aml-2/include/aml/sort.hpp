#pragma once

namespace aml
{
    template< typename... >
    struct list;

    template<typename...>
    struct sort;


    template<>
    struct sort<>
    {
    private:

        template<typename...>
        friend struct sort;

        template<typename List, template<typename...> class Less>
        struct sort_list_
        {
            struct sort_non_empty_list_
            {
                using pivot = typename List::head;

                template<typename X>
                using predicate = Less<X, pivot>;

                using partition = typename List::tail::template partition_with< predicate >;

                using lower = typename sort_list_<typename partition::accepted, Less>::type;
                using upper = typename sort_list_<typename partition::rejected, Less>::type;

                using type = typename upper::template apply<lower::template rcons<pivot>::template rcons>;
            };

            struct sort_empty_list_
            {
                using type = list<>;
            };

            using type = typename conditional< bool_<List::size() != 0>,
                                               sort_non_empty_list_,
                                               sort_empty_list_ >::type;
        };

    public:

        template< template<typename...> class >
        using with = list<>;
    };


    template<typename... X>
    struct sort
    {
        template< template<typename...> class Less >
        using with  =  typename sort<>::sort_list_< list<X...>, Less >::type;
    };
}

#include "./list.hpp"
