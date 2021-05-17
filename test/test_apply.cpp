#include "aml/apply.hpp"
#include "aml/string.hpp"

#include <type_traits>


namespace test::application
{
    template<typename... >
    struct F {};

        
    void test_single_application()
    {
        using aml::eval;
        
        using t0 = aml::apply<0, F>;
        using t1 = aml::apply<1, F>;

        static_assert(std::is_same< eval<      int>::type,              int >::value, "");
        static_assert(std::is_same< eval<       t0>::type,              F<> >::value, "");
        static_assert(std::is_same< eval<       t1>::type, aml::apply<0, F> >::value, "");
        static_assert(std::is_same< eval<eval<t1> >::type,         eval<t0> >::value, "");
        static_assert(std::is_same< eval<eval<t1> >::type::type,        F<> >::value, "");

        using s0 = aml::apply<0, F, int, char>;
        using s1 = aml::apply<1, F, int, char>;
        using F_t = F<int, char>;

        static_assert(std::is_same< eval<      int>::type,              int >::value, "");
        static_assert(std::is_same< eval<       s0>::type,              F_t >::value, "");
        static_assert(std::is_same< eval<       s1>::type, aml::apply<0, F, int, char> >::value, "");
        static_assert(std::is_same< eval<eval<s1> >::type,         eval<s0> >::value, "");
        static_assert(std::is_same< eval<eval<s1> >::type::type,        F_t >::value, "");               
    }

    
    void test_multiple_applications()
    {
        using t0 = aml::apply<0, F, int, aml::apply<2, F, char> >;
        using t1 = aml::eval<t0>;
        using t3 = aml::eval<aml::eval<t1> >;

        static_assert(std::is_same<t1::type, F<int, aml::apply<1, F, char> > >::value, "");
        static_assert(std::is_same<t3::type::type::type, F<int, F<char> > >::value, "");

        using t4 = aml::apply<0, aml::eval, aml::apply<2, F, int > >;
        using t5 = aml::eval<t4>;

        static_assert(std::is_same< t5::type, aml::eval< aml::apply<1, F, int> > >::value, "");
        static_assert(std::is_same< t5::type::type, aml::apply<0, F, int> >::value, "");
        static_assert(std::is_same< aml::eval<t5::type::type>::type, F<int> >::value, "");
    }

    void test_bra_and_ket()
    {
        using t0 = aml::bra<F>::ket<int, char>;
        using t1 = aml::ket<int, char>::bra<F>;

        using t2 = aml::bra<F, 1>::ket<int, char>;
        using t3 = aml::ket<int, char>::bra<F, 1>;
        
        static_assert(std::is_same<t0, t1>::value, "");
        static_assert(std::is_same<t2, t3>::value, "");

        static_assert(std::is_same<aml::eval<t0>::type, F<int, char> >::value, "");
    }
}

#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() = { test::application::test_single_application,
                             test::application::test_multiple_applications,
                             test::application::test_bra_and_ket };

    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
