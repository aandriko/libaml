/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/conslist.hpp"
#include "aml/string.hpp"

#include <type_traits>

#include <boost/core/demangle.hpp>
#include <iostream>

namespace test::conslist
{
    template<int n>
    struct type {};

    template<typename...>
    class Template {};


    void test_empty_list()
    {
        using aml::conslist;

        using empty = conslist<>;
        static_assert(empty::size() == 0, "");

        using list_one = conslist<>::cons<void>;
        using list_two = conslist<>::rcons<void>;

        static_assert(std::is_same< list_one, conslist<void> >::value, "");
        static_assert(std::is_same< list_two, conslist<void> >::value, "");
        static_assert(std::is_same< conslist<>::template apply<Template>, Template<> >::value, "");

        static_assert(std::is_same< empty, empty::reverse >::value, "");

        static_assert(std::is_same<conslist<>::lfold_with<Template, void***>, void***>::value, "");
        static_assert(std::is_same<conslist<>::rfold_with<Template, void***>, void***>::value, "");

        static_assert(std::is_same<aml::lfold<conslist<>, aml::function<Template>, void***>, void***>::value, "");
        static_assert(std::is_same<aml::rfold<conslist<>, aml::function<Template>, void***>, void***>::value, "");
    }


    void test_non_empty_list()
    {
        using aml::conslist;

        using list_t = conslist< type<1>, type<2>, type<3> >;

        static_assert(list_t::size() == 3, "");

        static_assert(std::is_same<list_t::cons<type<0> >,
                                   conslist< type<0>, type<1>, type<2>, type<3> > >::value, "");

        static_assert(std::is_same<list_t::rcons<type<0> >,
                                   conslist< type<1>, type<2>, type<3>, type<0> > >::value, "");

        static_assert(std::is_same<list_t::head, type<1> >::value, "");

        static_assert(std::is_same<list_t::tail, conslist<type<2>, type<3> > >::value, "");

        static_assert(std::is_same<list_t::reverse, conslist<type<3>, type<2>, type<1> > >::value, "");

        static_assert(std::is_same<list_t::apply<Template>, Template<type<1>, type<2>, type<3> > >::value, "");

        static_assert(std::is_same
                      <
                          list_t::lfold_with<Template, void***>,
                          Template<void***, Template< type<1>, Template<type<2>, type<3> > > >
                      >::value, "");

        static_assert(std::is_same
                      <
                          list_t::rfold_with<Template, void***>,
                          Template< type<1>, Template< type<2>, Template< type<3>, void***> > >
                      >::value, "");


        static_assert(std::is_same
                      <
                          list_t::lfold_with<Template, void***>,
                          aml::lfold< list_t, aml::function<Template>, void*** >
                      >::value, "");

        static_assert(std::is_same
                      <
                          list_t::rfold_with<Template, void***>,
                          aml::rfold< list_t, aml::function<Template>, void*** >
                      >::value, "");

    }

    template<auto >
    struct idx;

    void test_join()
    {
        using list_0 = aml::conslist<>;
        using list_1 = aml::conslist<idx<1> >;
        using list_2 = aml::conslist<idx<2>, idx<3> >;
        using list_3 = aml::conslist<idx<4>, idx<5>, idx<6> >;

        using j_0 = aml::conslist<>::join<>;
        using j_1 = aml::conslist<>::join<list_0>;
        using j_2 = aml::conslist<>::join<list_0, list_0>;
        using j_3 = aml::conslist<>::join<list_0, list_1>;
        using j_4 = aml::conslist<>::join<list_1, list_2, list_3>;
        using j_5 = aml::conslist<>::join<list_0, list_1, list_2, list_3>;

        static_assert(std::is_same<j_0, aml::conslist<>>::value, "");
        static_assert(std::is_same<j_1, aml::conslist<>>::value, "");
        static_assert(std::is_same<j_2, aml::conslist<>>::value, "");
        static_assert(std::is_same<j_3, aml::conslist<idx<1> > >::value, "");
        static_assert(std::is_same<j_4, aml::conslist<idx<1>, idx<2>, idx<3>, idx<4>, idx<5>, idx<6> > >::value, "");
        static_assert(std::is_same<j_5, j_4>::value, "");

        static_assert(std::is_same< aml::join<list_0, list_1, list_2, list_3>, j_4 >::value, "");
    }

}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::conslist::test_empty_list,
        test::conslist::test_non_empty_list,
        test::conslist::test_join

    };

    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
