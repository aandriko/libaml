#include "aml/conslist.hpp"
#include "aml/string.hpp"

#include <type_traits>


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

        static_assert(conslist<>::size() == 0, "");
    }

    void test_non_empty_list()
    {
        using aml::conslist;

        
        using list_t = conslist< type<1>, type<2>, type<3> >;

        static_assert(std::is_same<list_t::cons<type<0> >,
                                   conslist< type<0>, type<1>, type<2>, type<3> > >::value, "");

        static_assert(std::is_same<list_t::rcons<type<0> >,
                                   conslist< type<1>, type<2>, type<3>, type<0> > >::value, "");

        static_assert(std::is_same<list_t::head, type<1> >::value, "");

        static_assert(std::is_same<list_t::tail, conslist<type<2>, type<3> > >::value, "");

        static_assert(std::is_same<list_t::reverse, conslist<type<3>, type<2>, type<1> > >::value, "");

        static_assert(std::is_same<list_t::apply<Template>, Template<type<1>, type<2>, type<3> > >::value, "");
        
    }
    
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() = { test::conslist::test_empty_list, test::conslist::test_non_empty_list };

    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)  << " tests passed." << std::endl;

}
