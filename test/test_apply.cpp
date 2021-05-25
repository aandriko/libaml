#include "aml/apply.hpp"
#include "aml/string.hpp"
#include "aml/basic_types.hpp"

#include <type_traits>


namespace test::application
{
    template<typename... >
    struct F {};

        
    void test_single_application()
    {
        using aml::eval;
        
        using t0 = aml::eval< aml::apply<0, F> >;

        
        static_assert(std::is_same< eval<      int>,           int >::value, "");
        static_assert(std::is_same< eval<       t0>,           F<> >::value, "");

        using s0 = aml::apply<0, F, int, char>;
        using F_t = F<int, char>;

        static_assert(std::is_same< eval<      int>,              int >::value, "");
        static_assert(std::is_same< eval<       s0>,              F_t >::value, "");

    }


    template<typename...> class G {};

    
    template<typename... Args>
    struct H
    {
        static_assert(sizeof...(Args) !=  1, "");
        using type = typename aml::conslist<Args...>::head;
    };


    template<typename Arg>
    struct H<Arg>
    {
        // no type called "type" here.
    };


    template<int n, typename... Args>
    using type =
        aml::eval
        <
        typename aml::apply<0,
                            
                            aml::conditional,
                            
                            aml::bool_<sizeof...(Args) == 1>,

                            aml::apply<0, G, Args...>,
                            
                            aml::apply<n, aml::add_type, H<Args...> >

                          
                            
                            >
        >;
    
    
    void test_multiple_applications()
    {
        using namespace aml;
        
        static_assert(is_same< type<1, int>, G<int>>::eval(), "");
        static_assert(is_same< type<1, int, char>, int>::eval(), "");

        static_assert(is_same< type<0, int, char>, int>::eval(), "");
        // The next line must reulst in a compile-time error, because
        // the necessary delay of at least 1 is not added:
        //
        //static_assert(is_same< type<0, int>, G<int>>::eval(), "");



    }

    /*
    void test_bra_and_ket()
    {
        using t0 = aml::bra<F>::ket<int, char>;
        using t1 = aml::ket<int, char>::bra<F>;
        
        static_assert(std::is_same<t0, t1>::value, "");

        static_assert(std::is_same<aml::eval<t0>::type, F<int, char> >::value, "");

    }
    */
}

#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() = { test::application::test_single_application,
                             test::application::test_multiple_applications };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
