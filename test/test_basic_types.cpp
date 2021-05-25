#include "aml/apply.hpp"
#include "aml/string.hpp"
#include "aml/basic_types.hpp"

#include <type_traits>


namespace test::basic_types
{
    template<typename...> class F { };

    
    void test_enable_if()
    {
        using t = aml::enable<int>::if_<aml::true_>;

        static_assert( std::is_same<t, int>::value, "");

        using s = aml::if_<aml::true_>::enable<int>;

        static_assert( std::is_same<s, int>::value, "");
        
        // The following construction should lead to a compile-time error
        // using t = aml::enable<int>::if_<aml::false_>;   

        // enable<...>::if_<...> cannot be used for sfinae.
        // This is instead done with the types true_::sfinae == true_ and false_
        // (cf  void test_sfinae())
    }

    void test_bool()
    {
        static_assert(std::is_same<aml::bool_<true>, aml::true_>::value, "");
        static_assert(std::is_same<aml::bool_<false>, aml::false_>::value, "");
        

        static_assert(std::is_same<aml::all<>, aml::true_>::value, "");
        static_assert(std::is_same<aml::one<>, aml::false_>::value, "");
        static_assert(std::is_same<aml::none<>, aml::false_>::value, "");

        static_assert(std::is_same<aml::all< aml::true_, aml::true_, aml::true_ >, aml::true_>::value, "");
        static_assert(std::is_same<aml::one< aml::true_, aml::true_, aml::true_ >, aml::true_>::value, "");
        static_assert(std::is_same<aml::none< aml::true_, aml::true_, aml::true_ >, aml::false_>::value, "");

        static_assert(std::is_same<aml::all< aml::false_, aml::false_, aml::false_ >, aml::false_>::value, "");
        static_assert(std::is_same<aml::one< aml::false_, aml::false_, aml::false_ >, aml::false_>::value, "");
        static_assert(std::is_same<aml::none< aml::false_, aml::false_, aml::false_ >, aml::true_>::value, "");

        static_assert(std::is_same<aml::all< aml::true_, aml::false_, aml::false_ >, aml::false_>::value, "");
        static_assert(std::is_same<aml::one< aml::true_, aml::false_, aml::false_ >, aml::true_>::value, "");
        static_assert(std::is_same<aml::none< aml::true_, aml::false_, aml::false_ >, aml::true_>::value, "");
    }

    
    template<typename T>
    using is_int = aml::bool_< std::is_same<int, T>::value >;

    template<typename T, typename = typename is_int<T>::sfinae>
    aml::true_ sfinae_tag_used_(std::nullptr_t, std::nullptr_t);

    template<typename T>
    aml::false_ sfinae_tag_used_(std::nullptr_t, ...);

    template<typename T>
    using sfinae_tag_used = decltype(sfinae_tag_used_<T>(nullptr, nullptr));

    
    void test_sfinae()
    {
        static_assert(std::is_same<aml::true_, aml::true_::sfinae>::value, "");

        static_assert(sfinae_tag_used<int>::eval() == true, "");
        static_assert(sfinae_tag_used<char>::eval() == false, "");
    }


    void test_conditional()
    {
        struct t_true;
        struct t_false;

        static_assert( std::is_same<  aml::true_::conditional<t_true, t_false>,  t_true >::value, "");
        static_assert( std::is_same< aml::false_::conditional<t_true, t_false>, t_false >::value, "");

        using s_true  = aml::conditional< aml::true_, t_true, t_false>;
        using s_false = aml::conditional<aml::false_, t_true, t_false>;

        static_assert( std::is_same<t_true, s_true>::value, "");
        static_assert( std::is_same<t_false, s_false>::value, "");
    }


    void test_is_same()
    {

    }

    
    void test_add_type()
    {

    }

    
    void test_identity()
    {

    }

    void test_term()
    {

    }

    
    void test_bra_and_ket()
    {
        using t0 = aml::bra<F>::ket<aml::conslist<int, char>>;
        using t1 = aml::ket<aml::conslist<int, char>>::bra<F>;
        
        static_assert(std::is_same<t0, t1>::value, "");
        static_assert(std::is_same<t0, F<int, char> >::value, "");

    }

    
    void test_object()
    {
        
    }
    
}




#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::basic_types::test_enable_if,
        test::basic_types::test_bool,
        test::basic_types::test_sfinae,
        test::basic_types::test_conditional,
        test::basic_types::test_is_same,
        test::basic_types::test_add_type,
        test::basic_types::test_identity,
        test::basic_types::test_term,        
        test::basic_types::test_bra_and_ket,
        test::basic_types::test_object
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
