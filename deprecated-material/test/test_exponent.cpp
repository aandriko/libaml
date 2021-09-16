/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/exponent.hpp"
#include "aml/string.hpp"

#include <iostream>
#include <type_traits>


namespace test::exponent
{
    struct t3 {};
    struct t2 { using type = t3; };
    struct t1 { using type = t2; };
    struct t0 { using type = t1; };


    struct s3 { using type = s3; }; // cycle of typename type
    struct s2 { using type = s3; };
    struct s1 { using type = s2; };
    struct s0 { using type = s1; };


    template<typename... Arrgs>
    struct F_
    {
        struct type
        {
            enum { depth = 1 };
        };
    };

    template<typename... Args>
    struct F_<F_<Args...> >
    {
        struct type
        {
            enum { depth = 2 };
        };
    };

    template<typename... Args>
    struct F_<F_<F_<Args...> > >
    {
        struct type
        {
            enum { depth = 3 };
        };
    };

    template<typename... Args>
    struct F_<F_<F_<F_<Args...> > > >
    {
        using type = typename F_<F_<F_<Args...> > >::type;
    };

    template<typename... Args>
    using F = typename F_<Args...>::type;


    void power_of_type_test()
    {
        using namespace aml;

        static_assert( std::is_same<          identity<t0>,                t0 >::value, "");

        static_assert( std::is_same< power<exp<0>, t0>,  t0 >::value, "");

        static_assert( std::is_same< power<exp<1>, t0>,  t1 >::value, "");

        static_assert( std::is_same< power<exp<2>, t0>,  t2 >::value, "");



        static_assert( std::is_same< power<exp<infinity>, t0>, t3 >::value, ""); // no typename type in t3

        static_assert( std::is_same< power<exp<infinity>, s0>, s3 >::value, ""); // no typename s3

        struct loop
        {
            using type = loop;
        };

        // Check the infinite sequence loop::type::type::type.... with loop::type == loop
        static_assert(std::is_same< power<exp<infinity>, loop>, loop>::value, ""); 


    }


    void power_of_function_test()
    {
        using namespace aml;


        static_assert(std::is_same< function<>::power<exp<0>, F>::template apply_to<int>, int>::value,"");

        static_assert(std::is_same
                      <
                          function<>::power<exp<1>, F>::template apply_to<char, int*, double>,

                          F<char, int*, double>
                      >::value,"");


        static_assert(std::is_same
                      <
                          function<>::power<exp<2>, F>::template apply_to<char, int*, double>,

                          F<F<char, int*, double>>
                      >::value,"");


        static_assert(std::is_same
                      <
                          function<>::power<exp<3>, F>::template apply_to<char, int*, double>,

                          F<F<F<char, int*, double>>>
                      >::value,"");
    }

}


int main()
{
    void (*test_set[])() = { test::exponent::power_of_type_test, test::exponent::power_of_function_test };

    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
