#include "aml/list.hpp"
#include "aml/term_algebra.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

namespace test::list
{
    template<auto n>
    struct number
    {
        static constexpr auto eval() { return n; }
    };

    void test_head_and_tail()
    {
        using aml::_;

        using list_t = aml::head_and_tail<int, int*, int**, int***>;

        static_assert( std::is_same< list_t::head, int>::value );
        static_assert( std::is_same< aml::power< list_t::tail, _<2> >::return_, aml::head_and_tail<int**, int***> >::value);

        static_assert( std::is_same< aml::head<int, int*, int**>, int >::value );

        static_assert( std::is_same< aml::at< _<0>, int>, int >::value);

        static_assert( std::is_same< aml::at< _<3>, int, int*, int**, int***, int**** >, int***>::value );
        static_assert( std::is_same< aml::at< _<3>, int, int*, int**, int***>, int*** >::value );

        using t3 = aml::list<int*, int**, int***>;

        static_assert( std::is_same< typename t3::head, aml::list_head<t3> >::value );
        static_assert( std::is_same< aml::list_head<t3>, int* >::value );

        static_assert( std::is_same< typename t3::tail, aml::list_tail<t3> >::value );
        static_assert( std::is_same< aml::list_tail<t3>, aml::list<int**, int***> >::value);

        static_assert( std::is_same< typename t3::init, aml::list_init<t3> >::value );
        static_assert( std::is_same< aml::list_init<t3>, aml::list<int*, int**> >::value );


        static_assert( std::is_same< t3::last, aml::list_last<t3> >::value );
        static_assert( std::is_same< aml::list_last<t3>, int*** >::value );
    }

    template< typename... > struct F {};


    void test_apply()
    {
        using f0 = aml::list<>::apply<F>;
        using f1 = aml::list<int*>::apply<F>;
        using f2 = aml::list<int*, int**>::apply<F>;

        using f0_ = aml::list_apply< aml::list<>, aml::function<F> >;
        using f1_ = aml::list_apply< aml::list<int*>, aml::function<F> >;
        using f2_ = aml::list_apply< aml::list<int*, int**>, aml::function<F> >;

        using g0 = aml::list<>::pointwise_apply<F>;
        using g1 = aml::list<int*>::pointwise_apply<F>;
        using g2 = aml::list<int*, int**>::pointwise_apply<F>;

        using g0_ = aml::list_pointwise_apply< aml::list<>, aml::function<F> >;
        using g1_ = aml::list_pointwise_apply< aml::list<int*>, aml::function<F> >;
        using g2_ = aml::list_pointwise_apply< aml::list<int*, int**>, aml::function<F> >;

        static_assert(std::is_same<f0, f0_>::value);
        static_assert(std::is_same<f1, f1_>::value);
        static_assert(std::is_same<f2, f2_>::value);

        static_assert(std::is_same<g0, g0_>::value);
        static_assert(std::is_same<g1, g1_>::value);
        static_assert(std::is_same<g2, g2_>::value);

        static_assert(std::is_same< g0, aml::list<> >::value);
        static_assert(std::is_same< g1, aml::list<F<int*>> >::value);
        static_assert(std::is_same< g2, aml::list<F<int*>, F<int**> > >::value);
    }


    void test_drop_and_take()
    {
        using aml::_;

        using t3 = aml::list<int*, int**, int***>;

        static_assert( std::is_same< t3::drop< _<0> >, t3 >::value );


        static_assert( std::is_same< t3::drop< _<1> >, aml::list<int**, int***> >::value );
        static_assert( std::is_same< t3::drop< _<2> >, aml::list<       int***> >::value );
        static_assert( std::is_same< t3::drop< _<3> >, aml::list<             > >::value );

        static_assert( std::is_same< aml::list<>::take< _<0> >, aml::list<> >::value );

        static_assert( std::is_same< t3::take< _<1> >, aml::list< int*                > >::value );
        static_assert( std::is_same< t3::take< _<2> >, aml::list< int*, int**         > >::value );
        static_assert( std::is_same< t3::take< _<3> >, aml::list< int*, int**, int*** > >::value );
    }


    void test_cons()
    {
        using aml::_;

        using l0 = aml::list<>;
        using l0_a = l0::cons<>;
        using l0_b = l0::rcons<>;

        using l1 = l0::cons< _<1>, _<2>, _<3> >;
        using l1_a = l0::rcons<_<1>, _<2>, _<3> >;

        using l2 = l1::cons<>;
        using l2_a = l1::rcons<>;

        using l3 = l1::rcons< _<4>, _<5> >;
        using l4 = l1::cons< _<-1>, _< 0> >;

        static_assert( std::is_same<l0, l0_a>::value );
        static_assert( std::is_same<l0, l0_b>::value );
        static_assert( std::is_same<l1, l1_a>::value );

        static_assert( std::is_same<l2, l2_a>::value );
        static_assert( std::is_same<l3, aml::list< _<1>, _<2>, _<3>, _<4>, _<5> > >::value );

        static_assert( std::is_same<l4, aml::list< _<-1>, _<-0>,  _<1>, _<2>, _<3> > >::value );

        using l5 = aml::list_cons< aml::list< _<1>, _<2>, _<3> >  >;
        using l6 = aml::list_rcons< aml::list< _<1>, _<2>, _<3> > >;

        using l7 = aml::list_rcons< aml::list< _<1>, _<2>, _<3> >, _<4>, _<5>  >;
        using l8 = aml::list_cons< aml::list< _<1>, _<2>, _<3> >, _<-1>, _<0>  >;

        static_assert( std::is_same< l5, aml::list< _<1>, _<2>, _<3> > >::value );
        static_assert( std::is_same< l5, l6 >::value );

        static_assert( std::is_same< l7, aml::list< _<1>, _<2>, _<3>, _<4>, _<5> > >::value );
        static_assert( std::is_same< l8, aml::list< _<-1>, _<0>, _<1>, _<2>, _<3> > >::value );
    }

    void test_reverse()
    {
        using aml::_;

        using l1 = aml::list<>;
        using l2 = aml::list<>::reverse;
        using l3 = aml::list_reverse< aml::list<> >;

        using l4 = aml::list< _<1> >;
        using l5 = aml::list< _<1> >::reverse;
        using l6 = aml::list_reverse< l4 >;

        using l7 = aml::list< _<1>, _<2> >;
        using l8 = l7::reverse;
        using l9 = aml::list_reverse< l7 >;

        static_assert( std::is_same< l1, l2 >::value );
        static_assert( std::is_same< l1, l3 >::value );

        static_assert( std::is_same< l4, l5 >::value );
        static_assert( std::is_same< l4, l6 >::value );

        static_assert( std::is_same< l8, l9 >::value );

        static_assert( std::is_same< l8::reverse, l7 >::value );
    }

}


#include <iostream>
#include <string>


int main()
{

    void (*test_set[])() =
    {
        test::list::test_head_and_tail,
        test::list::test_apply,
        test::list::test_drop_and_take,
        test::list::test_cons,
        test::list::test_reverse,
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
