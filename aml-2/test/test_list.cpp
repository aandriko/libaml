#include "aml/list.hpp"

#include <type_traits>


namespace test::list
{
    template<auto n>
    struct number
    {
        static constexpr auto eval() { return n; }
    };

    void test_head_and_tail()
    {
        using list_t = aml::head_and_tail<int, int*, int**, int***>;

        static_assert( std::is_same< list_t::head, int>::value );
        static_assert( std::is_same< aml::power< list_t::tail, 2 >::return_, aml::head_and_tail<int**, int***> >::value);
        static_assert( std::is_same< aml::head<int, int*, int**>, int >::value );

        static_assert( std::is_same< aml::at<0, int>, int >::value);

        static_assert( std::is_same< aml::at<3, int, int*, int**, int***, int**** >, int***>::value );
        static_assert( std::is_same< aml::at<3, int, int*, int**, int***>, int*** >::value );

        static_assert( std::is_same< aml::at_l< number<3>, int, int*, int**, int***, int**** >, int***>::value );
        static_assert( std::is_same< aml::at_l< number<3>, int, int*, int**, int***>, int*** >::value );

        using t3 = aml::list<int*, int**, int***>;

        static_assert( std::is_same< aml::list_head<t3>, int* >::value );
        static_assert( std::is_same< aml::list_tail<t3>, aml::list<int**, int***> >::value);
        static_assert( std::is_same< aml::list_init<t3>, aml::list<int*, int**> >::value );
        //        static_assert( std::is_same< aml::list_last<t3>, int*** >::value );
        
        

        //        static_assert( std::is_same< t3::drop<0>, t3 >::value );
        //        static_assert( std::is_same< t3::drop<1>, aml::list<int**, int***> >::value );

        aml::list_head< aml::list<int, int**, int*** > > lh;
    }


}




#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::list::test_head_and_tail
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
