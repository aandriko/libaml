#include "aml/term_algebra.hpp"


#include <type_traits>


namespace test::term_algebra
{
    template<typename...> class H { };


    void test_term()
    {
        static_assert( std::is_same<  aml::function<H>::apply_to<int>, H<int> >::value );

        static_assert( std::is_same< aml::function<H>::template apply_to<>, H<> >::value );

        static_assert( std::is_same<aml::term<int>::function<char*>, char*>::value );

        static_assert( std::is_same<aml::term<H<>>::function<>, H<> >::value );

        static_assert( std::is_same< H<int, int*>,

                                     aml::term< H<int, int*> >::
                                     subterms::
                                     apply< aml::term< H<int, int*> >::function >
                       >::value );

        static_assert( std::is_same< aml::term<H<int, int*> >::subterms, aml::subterms<int, int*> >::value );

        static_assert( std::is_same< aml::term< H<> >::subterms, aml::subterms<> >::value);
        static_assert( std::is_same< aml::subterms<>::template apply<H>, H<> >::value );

        static_assert( std::is_same< aml::subterms<int, int*>::template apply<H>,
                                     H<int, int*> >::value );

        static_assert( std::is_same< aml::subterms<int>::head, int >::value );

        static_assert( std::is_same< aml::subterms<int>::tail, aml::subterms<> >::value );

        static_assert( std::is_same< aml::subterms<int, int*>::head, int >::value );

        static_assert( std::is_same< aml::subterms<int, int*>::tail, aml::subterms<int*> >::value );
    }


    void test_term_id()
    {
        static_assert( aml::term_id<int*> == aml::term_id<int*> );
        static_assert( aml::term_id<int*> != aml::term_id<void> );
    }


}




#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() =
    {
        test::term_algebra::test_term,
        test::term_algebra::test_term_id
    };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
