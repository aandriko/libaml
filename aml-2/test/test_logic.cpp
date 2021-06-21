#include "aml/logic.hpp"

#include <type_traits>


namespace test::logic
{
    template<typename...> class F { };


    void test_bool()
    {
        static_assert(std::is_same<aml::bool_<true>, aml::true_>::value);
        static_assert(std::is_same<aml::bool_<false>, aml::false_>::value);

        static_assert(aml::true_::eval() == true);
        static_assert(aml::false_::eval() == false);

        static_assert(std::is_same<aml::true_::sfinae, aml::true_>::value);
        static_assert(std::is_same<aml::true_::enable<int*>, int*>::value);

        static_assert(std::is_same< aml::true_::conditional<int*, int**>, int*>::value);
        static_assert(std::is_same<aml::false_::conditional<int*, int**>, int**>::value);
    }

    template<typename T>
    using is_int  =  aml::bool_< std::is_same<T, int>::value >;

    template<typename T, typename = typename is_int<T>::sfinae>
    aml::true_ sfinae_tag_used_(std::nullptr_t, std::nullptr_t);

    template<typename T>
    aml::false_ sfinae_tag_used_(std::nullptr_t, ...);

    template<typename T>
    using sfinae_tag_used = decltype(sfinae_tag_used_<T>(nullptr, nullptr));


    void test_sfinae()
    {
        static_assert(std::is_same<aml::true_, aml::true_::sfinae>::value );

        static_assert(sfinae_tag_used<int>::eval() == true );
        static_assert(sfinae_tag_used<char>::eval() == false );
    }


    template<typename T, typename R = typename is_int<T>::template enable<T> >
    R enable_(std::nullptr_t, std::nullptr_t);


    template<typename T>
    void enable_(std::nullptr_t, ...);


    template<typename T>
    using enable_t  =  decltype( enable_<T>(nullptr, nullptr) );


    void test_enable()
    {
        static_assert( std::is_same<enable_t<int>, int >::value );
        static_assert( std::is_same<enable_t<double>, void >::value );
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


    void test_not()
    {
        static_assert( std::is_same< aml::not_<aml::true_>, aml::false_ >::value );
        static_assert( std::is_same< aml::not_<aml::false_>, aml::true_ >::value );
    }

    void test_all_any_none()
    {
        static_assert( std::is_same<aml::all<>, aml::true_>::value );
        static_assert(std::is_same<aml::any<>, aml::false_>::value );
        static_assert(std::is_same<aml::none<>, aml::true_>::value );

        static_assert(std::is_same<aml::all< aml::true_, aml::true_, aml::true_ >, aml::true_ >::value );
        static_assert(std::is_same<aml::any< aml::true_, aml::true_, aml::true_ >, aml::true_>::value );
        static_assert(std::is_same<aml::none< aml::true_, aml::true_, aml::true_ >, aml::false_>::value );

        static_assert(std::is_same<aml::all< aml::false_, aml::false_, aml::false_ >, aml::false_>::value );
        static_assert(std::is_same<aml::any< aml::false_, aml::false_, aml::false_ >, aml::false_>::value );
        static_assert(std::is_same<aml::none< aml::false_, aml::false_, aml::false_ >, aml::true_>::value );

        static_assert(std::is_same<aml::all< aml::true_, aml::false_, aml::false_ >, aml::false_>::value );
        static_assert(std::is_same<aml::any< aml::true_, aml::false_, aml::false_ >, aml::true_>::value );
        static_assert(std::is_same<aml::none< aml::true_, aml::false_, aml::false_ >, aml::false_>::value );
    }


    template< typename T >
    using p1  =  aml::bool_< std::is_same<T, int*>::value >;

    template< typename T >
    using p2  =  aml::bool_< std::is_same<T, int**>::value >;

    template< typename T >
    using p3  =  aml::bool_< std::is_same<T, int***>::value >;

    template< typename T >
    using p12 = aml::bool_< p1<T>::eval() || p2<T>::eval() >;

    template< typename T >
    using p13 = aml::bool_< p1<T>::eval() || p3<T>::eval() >;

    template< typename T >
    using p123 = aml::bool_< p1<T>::eval() || p2<T>::eval() || p3<T>::eval() >;


    void test_predicates()
    {
        using all_0  =  aml::predicates::all<>;
        using any_0  =  aml::predicates::any<>;
        using none_0 =  aml::predicates::none<>;

        using all_1  =  aml::predicates::all<p1>;
        using any_1  =  aml::predicates::any<p1>;
        using none_1 =  aml::predicates::none<p1>;

        using all_2  =  aml::predicates::all<p1, p2>;
        using any_2  =  aml::predicates::any<p1, p2>;
        using none_2 =  aml::predicates::none<p1, p2>;

        using all_3  =  aml::predicates::all<p1, p2, p3>;
        using any_3  =  aml::predicates::any<p1, p2, p3>;
        using none_3 =  aml::predicates::none<p1, p2, p3>;

        using t1  =  int*;
        using t2  =  int**;
        using t3  =  int***;
        using t4  =  char;

        using all_01 = all_0::apply_to<t1>;
        using all_02 = all_0::apply_to<t2>;
        using all_03 = all_0::apply_to<t3>;
        using all_04 = all_0::apply_to<t4>;

        static_assert( all_01::eval() );
        static_assert( all_02::eval() );
        static_assert( all_03::eval() );
        static_assert( all_04::eval() );

        using any_01 = any_0::apply_to<t1>;
        using any_02 = any_0::apply_to<t2>;
        using any_03 = any_0::apply_to<t3>;
        using any_04 = any_0::apply_to<t4>;

        static_assert( ! any_01::eval() );
        static_assert( ! any_02::eval() );
        static_assert( ! any_03::eval() );
        static_assert( ! any_04::eval() );

        using none_01 = none_0::apply_to<t1>;
        using none_02 = none_0::apply_to<t2>;
        using none_03 = none_0::apply_to<t3>;
        using none_04 = none_0::apply_to<t4>;

        static_assert( none_01::eval() );
        static_assert( none_02::eval() );
        static_assert( none_03::eval() );
        static_assert( none_04::eval() );

        //

        using all_11 = all_1::apply_to<t1>;
        using all_12 = all_1::apply_to<t2>;
        using all_13 = all_1::apply_to<t3>;
        using all_14 = all_1::apply_to<t4>;

        static_assert(   all_11::eval() );
        static_assert( ! all_12::eval() );
        static_assert( ! all_13::eval() );
        static_assert( ! all_14::eval() );

        using any_11 = any_1::apply_to<t1>;
        using any_12 = any_1::apply_to<t2>;
        using any_13 = any_1::apply_to<t3>;
        using any_14 = any_1::apply_to<t4>;

        static_assert(   any_11::eval() );
        static_assert( ! any_12::eval() );
        static_assert( ! any_13::eval() );
        static_assert( ! any_14::eval() );

        using none_11 = none_1::apply_to<t1>;
        using none_12 = none_1::apply_to<t2>;
        using none_13 = none_1::apply_to<t3>;
        using none_14 = none_1::apply_to<t4>;

        static_assert( ! none_11::eval() );
        static_assert( none_12::eval() );
        static_assert( none_13::eval() );
        static_assert( none_14::eval() );

        //

        using all_21 = all_2::apply_to<t1>;
        using all_22 = all_2::apply_to<t2>;
        using all_23 = all_2::apply_to<t3>;
        using all_24 = all_2::apply_to<t4>;

        static_assert( ! all_21::eval() );
        static_assert( ! all_22::eval() );
        static_assert( ! all_23::eval() );
        static_assert( ! all_24::eval() );

        using any_21 = any_2::apply_to<t1>;
        using any_22 = any_2::apply_to<t2>;
        using any_23 = any_2::apply_to<t3>;
        using any_24 = any_2::apply_to<t4>;

        static_assert(   any_21::eval() );
        static_assert(   any_22::eval() );
        static_assert( ! any_23::eval() );
        static_assert( ! any_24::eval() );

        using none_21 = none_2::apply_to<t1>;
        using none_22 = none_2::apply_to<t2>;
        using none_23 = none_2::apply_to<t3>;
        using none_24 = none_2::apply_to<t4>;

        static_assert( ! none_21::eval() );
        static_assert( ! none_22::eval() );
        static_assert(   none_23::eval() );
        static_assert(   none_24::eval() );

        //

        using all_31 = all_3::apply_to<t1>;
        using all_32 = all_3::apply_to<t2>;
        using all_33 = all_3::apply_to<t3>;
        using all_34 = all_3::apply_to<t4>;

        static_assert( ! all_31::eval() );
        static_assert( ! all_32::eval() );
        static_assert( ! all_33::eval() );
        static_assert( ! all_34::eval() );

        using any_31 = any_3::apply_to<t1>;
        using any_32 = any_3::apply_to<t2>;
        using any_33 = any_3::apply_to<t3>;
        using any_34 = any_3::apply_to<t4>;

        static_assert(   any_31::eval() );
        static_assert(   any_32::eval() );
        static_assert(   any_33::eval() );
        static_assert( ! any_34::eval() );

        using none_31 = none_3::apply_to<t1>;
        using none_32 = none_3::apply_to<t2>;
        using none_33 = none_3::apply_to<t3>;
        using none_34 = none_3::apply_to<t4>;

        static_assert( ! none_31::eval() );
        static_assert( ! none_32::eval() );
        static_assert( ! none_33::eval() );
        static_assert(   none_34::eval() );


        using s1   =  aml::predicates::all<p123, p12, p1>::apply_to<t1>;

        using s2   =  aml::predicates::any<p123, p12, p1>::apply_to<t1>;

        using s3   =  aml::predicates::none<p123, p12, p1>::apply_to<t1>;

        using s4   =  aml::predicates::all<p123, p12, p1>::apply_to<t2>;

        using s5   =  aml::predicates::any<p123, p12, p1>::apply_to<t2>;

        using s6   =  aml::predicates::none<p123, p12, p1>::apply_to<t2>;

        using s7   =  aml::predicates::all<p123, p12, p1>::apply_to<t3>;

        using s8   =  aml::predicates::any<p123, p12, p1>::apply_to<t3>;

        using s9   =  aml::predicates::none<p123, p12, p1>::apply_to<t3>;

        using s10  =  aml::predicates::all<p123, p12, p1>::apply_to<t4>;

        using s11  =  aml::predicates::any<p123, p12, p1>::apply_to<t4>;

        using s12  =  aml::predicates::none<p123, p12, p1>::apply_to<t4>;


        static_assert(   std::is_same<s1, aml::true_>::value );

        static_assert(   std::is_same<s2, aml::true_>::value );

        static_assert( ! std::is_same<s3, aml::true_>::value );

        static_assert( ! std::is_same<s4, aml::true_>::value );

        static_assert(   std::is_same<s5, aml::true_>::value );

        static_assert( ! std::is_same<s6, aml::true_>::value );

        static_assert( ! std::is_same<s7, aml::true_>::value );

        static_assert(   std::is_same<s8, aml::true_>::value );

        static_assert( ! std::is_same<s9, aml::true_>::value );

        static_assert( ! std::is_same<s10, aml::true_>::value );

        static_assert( ! std::is_same<s11, aml::true_>::value );

        static_assert(   std::is_same<s12, aml::true_>::value );
    }



}




#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::logic::test_bool,
        test::logic::test_sfinae,
        test::logic::test_enable,
        test::logic::test_conditional,
        test::logic::test_not,
        test::logic::test_all_any_none,
        test::logic::test_predicates
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
