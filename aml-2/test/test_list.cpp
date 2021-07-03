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


    template<typename List, typename X>
    using G = typename List::template rcons<  aml::num< -X::eval() >  >;


    void test_fold_left()
    {
        using aml::_;

        using l0  =  aml::list<>;
        using l1  =  aml::list< _<1> >;
        using l2  =  aml::list< _<1>, _<2> >;
        using l4  =  aml::list< _<1>, _<2>, _<3>, _<4> >;


        using f0  =  l0::lfold_with< G, aml::list<> >;
        using r0  =  aml::list< >;

        static_assert( std::is_same< f0, r0 >::value );


        using f1  =  l1::lfold_with< G, aml::list<> >;
        using r1  =  aml::list< _<-1> >;

        static_assert( std::is_same< f1, r1 >::value );


        using f2  =  l2::lfold_with< G, aml::list<> >;
        using r2  =  aml::list< _<-1>, _<-2> >;

        static_assert( std::is_same< f2, r2 >::value );


        using f4  =  l4::lfold_with< G, aml::list<> >;
        using r4  =  aml::list< _<-1>, _<-2>, _<-3>, _<-4> >;

        static_assert( std::is_same< f4, r4 >::value );
    }


    template<typename X, typename List>
    using H  =  typename List::template cons<  aml::num< -X::eval() >  >;


    void test_fold_right()
    {
        using aml::_;

        using l0  =  aml::list<>;
        using l1  =  aml::list< _<1> >;
        using l2  =  aml::list< _<1>, _<2> >;
        using l4  =  aml::list< _<1>, _<2>, _<3>, _<4> >;


        using f0  =  l0::rfold_with< H, aml::list<> >;
        using r0  =  aml::list< >;

        static_assert( std::is_same< f0, r0 >::value );


        using f1  =  l1::rfold_with< H, aml::list<> >;
        using r1  =  aml::list< _<-1> >;

        static_assert( std::is_same< f1, r1 >::value );


        using f2  =  l2::rfold_with< H, aml::list<> >;
        using r2  =  aml::list< _<-1>, _<-2> >;

        static_assert( std::is_same< f2, r2 >::value );


        using f4  =  l4::rfold_with< H, aml::list<> >;
        using r4  =  aml::list< _<-1>, _<-2>, _<-3>, _<-4> >;

        static_assert( std::is_same< f4, r4 >::value );
    }


    void test_scan_left()
    {
        using aml::_;

        using l0  =  aml::list<>;
        using l1  =  aml::list< _<1> >;
        using l2  =  aml::list< _<1>, _<2> >;
        using l4  =  aml::list< _<1>, _<2>, _<3>, _<4> >;


        using f0  =  l0::lscan_with< G, aml::list<> >;
        using r0  =  aml::list< aml::list<> >;

        static_assert( std::is_same< f0, r0 >::value );


        using f1  =  l1::lscan_with< G, aml::list<> >;
        using r1  =  aml::list<  aml::list<>,  aml::list< _<-1> >  >;

        static_assert( std::is_same< f1, r1 >::value );


        using f2  =  l2::lscan_with< G, aml::list<> >;
        using r2  =  aml::list< aml::list<>, aml::list< _<-1> >, aml::list< _<-1>, _<-2> > >;

        static_assert( std::is_same< f2, r2 >::value );


        using f4  =  l4::lscan_with< G, aml::list<> >;
        using r4  =  aml::list<  aml::list<>,
                                 aml::list< _<-1> >,
                                 aml::list< _<-1>, _<-2> >,
                                 aml::list< _<-1>, _<-2>, _<-3> >,
                                 aml::list< _<-1>, _<-2>, _<-3>, _<-4> >  >;

        static_assert( std::is_same< f4, r4 >::value );
    }

    void test_scan_right()
    {
        using aml::_;

        using l0  =  aml::list<>;
        using l1  =  aml::list< _<1> >;
        using l2  =  aml::list< _<1>, _<2> >;
        using l4  =  aml::list< _<1>, _<2>, _<3>, _<4> >;


        using f0  =  l0::rscan_with< H, aml::list<> >;
        using r0  =  aml::list< aml::list<> >;

        static_assert( std::is_same< f0, r0 >::value );

        using f1  =  l1::rscan_with< H, aml::list<> >;
        using r1  =  aml::list<  aml::list<>,  aml::list< _<-1> >  >::reverse;

        static_assert( std::is_same< f1, r1 >::value );

        using f2  =  l2::rscan_with< H, aml::list<> >;
        using r2  =  aml::list<  aml::list< _<-1>, _<-2> >,
                                 aml::list< _<-2> > ,
                                 aml::list<>  >;

        static_assert( std::is_same< f2, r2 >::value );


        using f4  =  l4::rscan_with< H, aml::list<> >;
        using r4  =  aml::list<  aml::list<  _<-1>, _<-2>, _<-3>, _<-4>  >,
                                 aml::list<         _<-2>, _<-3>, _<-4>  >,
                                 aml::list<                _<-3>, _<-4>  >,
                                 aml::list<                       _<-4>  >,
                                 aml::list<                              >   >;

        static_assert( std::is_same< f4, r4 >::value );

    }

    template<typename B, typename A>
    using multiply = aml::list< aml::num< B::eval() * A::eval() >, aml::num< -A::eval() > >;

    void test_map_accum_left()
    {

        using aml::_;


        using macl_0  =  aml::list<>::map_accum_left_with<multiply, _<1> >;

        static_assert( std::is_same<macl_0, aml::list< _<1>, aml::list<>  > >::value );


        using macl_1  =  aml::list< _<1> >::map_accum_left_with< multiply, _<1> >;

        static_assert( std::is_same<macl_1, aml::list< _<1>, aml::list<_<-1> >  > >::value );


        using macl_2  =  aml::list< _<1>, _<2> >::map_accum_left_with< multiply, _<1> >;

        static_assert( std::is_same<macl_2, aml::list< _<2>, aml::list<_<-1>, _<-2> >  > >::value );


        using macl_3  =  aml::list< _<1>, _<2>, _<3>  >::map_accum_left_with< multiply, _<1> >;

        static_assert( std::is_same<macl_3, aml::list< _<6>, aml::list<_<-1>, _<-2>, _<-3> >  > >::value );
    }


    void test_map_accum_right()
    {

        using aml::_;


        using macr_0  =  aml::list<>::map_accum_right_with<multiply, _<1> >;

        static_assert( std::is_same<macr_0, aml::list< _<1>, aml::list<>  > >::value );


        using macr_1  =  aml::list< _<1> >::map_accum_right_with< multiply, _<1> >;

        static_assert( std::is_same<macr_1, aml::list< _<1>, aml::list<_<-1> >  > >::value );


        using macr_2  =  aml::list< _<1>, _<2> >::map_accum_right_with< multiply, _<1> >;

        static_assert( std::is_same<macr_2, aml::list< _<2>, aml::list<_<-2>, _<-1> >  > >::value );


        using macr_3  =  aml::list< _<1>, _<2>, _<3>  >::map_accum_right_with< multiply, _<1> >;

        static_assert( std::is_same<macr_3, aml::list< _<6>, aml::list<_<-3>, _<-2>, _<-1> >  > >::value );
    }


    template<typename N>
    using is_even = aml::bool_< (N::eval() & 1) == 0 >;


    void test_partition()
    {
        using aml::_;

        using l0    =  aml::list<>::partition_with<is_even>;
        using l1_a  =  aml::list< _<1> >::partition_with<is_even>;
        using l1_b  =  aml::list< _<2> >::partition_with<is_even>;
        using l2_a  =  aml::list< _<1>, _<2> >::partition_with<is_even>;
        using l2_b  =  aml::list< _<1>, _<3> >::partition_with<is_even>;
        using l2_c  =  aml::list< _<1>, _<4> >::partition_with<is_even>;
        using l2_d  =  aml::list< _<2>, _<4> >::partition_with<is_even>;

        using l4    =  aml::list< _<1>, _<2>, _<4>, _<3> >::partition_with<is_even>;

        using a0    =  aml::list<>;
        using r0    =  aml::list<>;

        using a1_a  =  aml::list<>;
        using r1_a  =  aml::list< _<1> >;

        using a1_b  =  aml::list< _<2> >;
        using r1_b  =  aml::list<>;

        using a2_a  =  aml::list< _<2> >;
        using r2_a  =  aml::list< _<1> >;

        using a2_b  =  aml::list<>;
        using r2_b  =  aml::list< _<1>, _<3> >;

        using a2_c  =  aml::list< _<4> >;
        using r2_c  =  aml::list< _<1> >;

        using a2_d  =  aml::list< _<2>, _<4> >;
        using r2_d  =  aml::list< >;

        static_assert( std::is_same< a0, l0::accepted >::value);
        static_assert( std::is_same< r0, l0::rejected >::value);


        static_assert( std::is_same< a1_a, l1_a::accepted >::value);
        static_assert( std::is_same< r1_a, l1_a::rejected >::value);


        static_assert( std::is_same< a1_b, l1_b::accepted >::value);
        static_assert( std::is_same< r1_b, l1_b::rejected >::value);


        static_assert( std::is_same< a2_a, l2_a::accepted >::value);
        static_assert( std::is_same< r2_a, l2_a::rejected >::value);


        static_assert( std::is_same< a2_b, l2_b::accepted >::value);
        static_assert( std::is_same< r2_b, l2_b::rejected >::value);


        static_assert( std::is_same< a2_c, l2_c::accepted >::value);
        static_assert( std::is_same< r2_c, l2_c::rejected >::value);


        static_assert( std::is_same< a2_d, l2_d::accepted >::value);
        static_assert( std::is_same< r2_d, l2_d::rejected >::value);

        static_assert( std::is_same< l4::accepted, aml::list< _<2>, _<4> > >::value);
        static_assert( std::is_same< l4::rejected, aml::list< _<1>, _<3> > >::value);

    }


    void test_split_by_first_occurence_of()
    {
        using aml::_;

        using s0    =  aml::list<>::split_by_first_occurence_of< is_even >;
        using s1_a  =  aml::list< _<2> >::split_by_first_occurence_of< is_even >;
        using s1_r  =  aml::list< _<1> >::split_by_first_occurence_of< is_even >;

        using s2    =  aml::list< _<1>, _<2> >::split_by_first_occurence_of<is_even>;
        using s3    =  aml::list< _<2>, _<1> >::split_by_first_occurence_of<is_even>;

        using s4    =  aml::list< _<1>, _<3>, _<2>, _<5> >::split_by_first_occurence_of<is_even>;

        static_assert( std::is_same<s0::prefix, aml::list<> >::value );
        static_assert( std::is_same<s0::suffix, aml::list<> >::value );

        static_assert( std::is_same< s1_a::prefix, aml::list<> >::value );
        static_assert( std::is_same< s1_a::suffix, aml::list<_<2>> >::value );

        static_assert( std::is_same< s1_r::prefix, aml::list<_<1>> >::value );
        static_assert( std::is_same< s1_r::suffix, aml::list< > >::value );

        static_assert( std::is_same< s2::prefix, aml::list< _<1> > >::value );
        static_assert( std::is_same< s2::suffix, aml::list< _<2> > >::value );

        static_assert( std::is_same< s3::prefix, aml::list<> >::value );
        static_assert( std::is_same< s3::suffix, aml::list< _<2>, _<1> > >::value );

        static_assert( std::is_same< s4::prefix, aml::list< _<1>, _<3> > >::value );
        static_assert( std::is_same< s4::suffix, aml::list< _<2>, _<5> > >::value );
    }

    void test_take_and_drop_while()
    {
        using aml::_;

        using l0 = aml::list<>;
        using l1 = aml::list< _<1> >;
        using l2 = aml::list< _<0> >;
        using l3 = aml::list< _<0>, _<1> >;
        using l4 = aml::list< _<1>, _<0> >;
        using l5 = aml::list< _<0>, _<2>, _<4>, _<3>, _<6> >;

        using t0  = l0::take_while< is_even >;
        using t1  = l1::take_while< is_even >;
        using t2  = l2::take_while< is_even >;
        using t3  = l3::take_while< is_even >;
        using t4  = l4::take_while< is_even >;
        using t5  = l5::take_while< is_even >;

        using d0  = l0::drop_while< is_even >;
        using d1  = l1::drop_while< is_even >;
        using d2  = l2::drop_while< is_even >;
        using d3  = l3::drop_while< is_even >;
        using d4  = l4::drop_while< is_even >;
        using d5  = l5::drop_while< is_even >;

        static_assert( std::is_same< t0, aml::list<> >::value );
        static_assert( std::is_same< t1, aml::list<> >::value );
        static_assert( std::is_same< t2, aml::list<_<0>>>::value );
        static_assert( std::is_same< t3, aml::list<_<0>> >::value );
        static_assert( std::is_same< t4, aml::list< > >::value );
        static_assert( std::is_same< t5, aml::list< _<0>, _<2>, _<4> > >::value );

        static_assert( std::is_same< d0, aml::list<> >::value );
        static_assert( std::is_same< d1, aml::list<_<1>> >::value );
        static_assert( std::is_same< d2, aml::list<> >::value );
        static_assert( std::is_same< d3, aml::list<_<1>> >::value );
        static_assert( std::is_same< d4, aml::list< _<1>, _<0> > >::value );
        static_assert( std::is_same< d5, aml::list< _<3>, _<6> > >::value );
    }



    //    template< typename X, typename Y, typename... >
    //    template<typename X, typename Y, typename...>
    //    using less = aml::bool_< (X::eval() < Y::eval()) >;

    template<typename X, typename Y>
    using less  = aml::bool_< (X::eval() < Y::eval()) >;


    template<typename... X>
    using size_less  =  aml::bool_< (sizeof( aml::at< aml::_<0>, X...>) < sizeof( aml::at< aml::_<1>, X...>)) >;

    void test_sort()
    {

        using aml::_;

        using unsorted_5 = aml::list< _<3>, _<2>, _<1>, _<4>, _<0> >;
        using sorted_5 = unsorted_5::sort_with<less>;
        static_assert( std::is_same< sorted_5, aml::list< _<0>, _<1>, _<2>, _<3>, _<4> > >::value );

        static_assert( std::is_same< aml::list<>::sort_with<less>, aml::list<> >::value );

        // test stability
        using list  =  aml::list< unsigned char, unsigned int, int, signed char, unsigned int, int, unsigned int>;
        using sorted_list  =  aml::list< unsigned char, signed char,
                                         unsigned int, int, unsigned int, int, unsigned int >;

        static_assert( std::is_same< list::sort_with<size_less>, sorted_list >::value );

        static_assert( std::is_same< aml::list_sort<list, aml::function<size_less> >, sorted_list>::value );

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
        test::list::test_fold_left,
        test::list::test_fold_right,
        test::list::test_scan_left,
        test::list::test_scan_right,
        test::list::test_map_accum_left,
        test::list::test_map_accum_right,
        test::list::test_partition,
        test::list::test_split_by_first_occurence_of,
        test::list::test_take_and_drop_while,
        test::list::test_sort

    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
