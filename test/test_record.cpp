/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Andreas Milton Maniotis.
//
// Email: andreas.maniotis@gmail.com
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////////////////////


#include "aml/adt/record.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

#include <string>
#include <memory>

#include <cassert>

namespace test::record
{
    struct hello;
    struct world;


    template<  typename T,
               typename  =  decltype ( std::declval<T>() = std::declval<T const&>() ) >
    aml::true_ has_copy_assignment_( std::nullptr_t, std::nullptr_t );


    template<  typename T  >
    aml::false_ has_copy_assignment_( std::nullptr_t, ... );


    template<  typename T,
               typename  =  decltype ( std::declval<T>() = std::declval<T&&>() ) >
    aml::true_ has_move_assignment_( std::nullptr_t, std::nullptr_t );


    template<  typename T  >
    aml::false_ has_move_assignment_( std::nullptr_t, ... );


    template<  typename T  >
    using has_copy_assignment  =  decltype( has_copy_assignment_<T>(nullptr, nullptr) );


    template<  typename T  >
    using has_move_assignment  =  decltype( has_move_assignment_<T>(nullptr, nullptr) );


    void test_adt_entry()
    {
        using indexed_t = aml::adt::entry<hello, std::unique_ptr<std::string>>;

        static_assert( has_copy_assignment<indexed_t>::eval() == false );
        static_assert( has_move_assignment<indexed_t>::eval() == true  );


        indexed_t a( new std::string("hello") );
        indexed_t b( new std::string("world"));


        b = std::move(a) ;
        a.ref() = std::make_unique<std::string>("hello again");

        assert( *(a.ref()) == std::string("hello again") );

        b = decltype(b)(a.rref());

        assert( a.cref() == nullptr );


        aml::adt::entry<world, std::string> w1("hello");
        aml::adt::entry<world, std::string> w2("world");

        aml::adt::entry<world, std::string> w3(w2);

        assert( static_cast<decltype(w2) const&>(w2).ref() == std::string("world") );

        aml::adt::entry<world, std::string> w4(std::move(w2));

        assert( w2.ref() == "" );

        w2 = w4;

        assert( w2.ref() == "world" );

        w2 = std::move(w4);

        assert( w2.ref() == "world" );
        assert( w4.ref() == "" );


        static_assert( std::is_same< decltype(w2.ref()), std::string & >::value );
        static_assert( std::is_same< decltype(w2.cref()), std::string const & >::value );
        static_assert( std::is_same< decltype(w2.rref()), std::string && >::value );

        static_assert( std::is_same< decltype(static_cast<decltype(w2) const & >(w2).ref()),
                                     std::string const & >::value );
    }

    void test_record()
    {
        struct tag;

        using r0_t    =  aml::adt::record<>;
        using r1_t    =  aml::adt::record< aml::entry<hello, int> >;


        using r3_a_t  =  aml::adt::record< aml::entry<hello, std::string>
                                         , aml::entry<world, double >
                                         , aml::entry<tag,   char >
                                         >;

        using r3_b_t =  aml::adt::record< aml::entry<hello, std::string>
                                        , aml::entry<world, std::unique_ptr<double> >
                                        >;

        {
            r0_t x0;
            r0_t x1(x0);
            r0_t x2( std::move(x0) );

            assert(x2 == x1 && x2 == x0 );

            assert( x0 == x1 );
            assert( !( x0 != x1) );

        }

        {
            r1_t y0;
            assert( y0.ref<hello>() == 0 );

            r1_t y1(2);
            assert( y1.ref<hello>() == 2 );

            assert( y0 != y1 );
            assert( y0 == y0 );

            y0 = std::move(y1);
            assert (y1 == r1_t(2) );

            y1 = r1_t(-1);

            auto y2 = y1;

            assert( y2.cref<hello>() == -1 );

            assert( y2 == y1 );
        }


        {
            r3_a_t z0( "hello", 2.4, 'a');


            static_assert( has_copy_assignment<r3_a_t>::eval() == true );
            static_assert( has_move_assignment<r3_a_t>::eval() == true );

            static_assert( has_copy_assignment<r3_b_t>::eval() == false );
            static_assert( has_move_assignment<r3_b_t>::eval() == true );

            auto z1(z0);

            assert(z1 == z0);
            assert(! (z1 != z0) );

            r3_a_t z2(std::move(z0));


            assert(z2 != z0);
            assert(! (z1 == z0) );

            assert(z2 == z2 && z1 == z1 && z0 == z0 );


            r3_b_t w0( "HELLO", new double(2.4));
            r3_b_t w1( "xyz",   new double(-4.3));

            r3_b_t w2;
            assert(w2 == r3_b_t("", nullptr) );

            w2 = std::move(w0);
            r3_b_t w4{ std::move(w1) };

            assert(w0 == r3_b_t("", nullptr));
            assert(w1 == w0);

            assert(  w4.cref<hello>() == "xyz" );
            assert( *w4.cref<world>()  == -4.3 );

            assert( w1 == r3_b_t("", nullptr) );
        }

    }

    void test_permuted_record()
    {
        struct tag;

        using r3  =  aml::adt::record< aml::entry<hello, std::string>
                                     , aml::entry<world, double >
                                     , aml::entry<tag,   char >
                                     >;

        using r3p =  aml::adt::record< aml::entry<hello, std::string>
                                     , aml::entry<tag,   char >
                                     , aml::entry<world, double >
                                     >;

        r3  z0( "hello", 2.4, 'a' );
        r3p z1( "hello", 'a', 2.4 );
        //        r3  z3(z1);
        r3 z3 = z1.extract<r3>();

    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
     test::record::test_adt_entry,
     test::record::test_record,
     test::record::test_permuted_record
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
