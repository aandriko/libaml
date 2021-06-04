#include "aml/dictionary.hpp"


#include <type_traits>

#include <iostream>
#include <type_traits>

#include <boost/core/demangle.hpp>

namespace test::dictionary
{
    void empty_dictionary_test()
    {
        using d = aml::dictionary<>;

        static_assert(std::is_same<d::keys, aml::conslist<> >::value, "");
        static_assert(std::is_same<d::values, aml::conslist<> >::value, "");
        static_assert(std::is_same<d::entries, aml::conslist<> >::value, "");
        static_assert(d::contains_all_keys<>::eval(), "");
        static_assert(d::contains_all_keys<void, int>::eval() == false, "");
        static_assert(std::is_same< d::subdictionary<>, d>::value, "");
        // using error_1 = d::subdictionary<int>;
        // using error_2 = d::map<int>
        static_assert(std::is_same<d::map<>, aml::conslist<> >::value, "");
        static_assert(std::is_same<d::partial_map<>, aml::conslist<> >::value, "");
        static_assert(std::is_same<d::partial_map<int, double>, aml::conslist< aml::conslist<>, aml::conslist<> > >::value, "");

        using d1 = aml::dictionary<>::add_entries< aml::entry<int, void> >;
        using d2 = aml::dictionary<>::add_entries< aml::entry<int, void>, aml::entry<int*, void*> >;

        static_assert(std::is_same<d1, aml::dictionary< aml::entry<int, void> > >::value, "");
        static_assert(std::is_same<d2, aml::dictionary< aml::entry<int, void>, aml::entry<int*, void*> > >::value, "");
        static_assert(std::is_same<d, d::erase_keys<>>::value, "");
        // using error_3 =  d::erase_keys<int, double>;

        static_assert(std::is_same<d, d::partially_erase_keys<>>::value, "");
        static_assert(std::is_same<d, d::partially_erase_keys<int, double>>::value, "");

        static_assert(d::size() == 0, "");
    }


    void filled_dictionary_test()
    {
        using d = aml::dictionary< aml::entry<int, void>, aml::entry<int*, void*>, aml::entry<int**, void**> >;

        static_assert(d::size() == 3);

        static_assert( std::is_same<d::keys, aml::conslist<int, int*, int**> >::value, "");
        static_assert( std::is_same<d::values, aml::conslist<void, void*, void**> >::value, "");
        static_assert( std::is_same<d::entries, aml::conslist< aml::entry<int, void>, aml::entry<int*, void*>, aml::entry<int**, void**> > >::value, "");

        static_assert(d::contains_all_keys<int*, int>::eval(), "");
        static_assert(d::contains_all_keys<int, double>::eval() == false, "");

        using e = d::subdictionary<int**, int>;

        static_assert( std::is_same
                       <
                           d::subdictionary<int**, int>,
                           aml::dictionary< aml::entry<int, void>, aml::entry<int**, void**> >
                       >::value, "");


        static_assert( std::is_same< d::map<>, aml::conslist<> >::value, "");

        static_assert( std::is_same< d::partial_map<int**, float, int>,
                       aml::conslist< aml::conslist<void**>, aml::conslist<>, aml::conslist<void> > >::value, "");

        static_assert( std::is_same< d::partial_map<int, float, int**>,
                       aml::conslist< aml::conslist<void>, aml::conslist<>, aml::conslist<void**> > >::value, "");


        // using error_4 = d::map<int**, float, int>;

        static_assert( std::is_same< d::map<int**, int>, aml::conslist<void**, void> >::value, "");
        static_assert( std::is_same< d::map<int, int**>, aml::conslist<void, void**> >::value, "");

        static_assert( std::is_same< d::add_entries< aml::entry<int, float> >,
                       aml::dictionary< aml::entry<int, float>, aml::entry<int*, void*>, aml::entry<int**, void**> > >::value, "");

        static_assert( std::is_same< d::add_entries< aml::entry<int*, float> >,
                       aml::dictionary< aml::entry<int, void>, aml::entry<int*, float>, aml::entry<int**, void**> > >::value, "");

        static_assert( std::is_same< d::add_entries< aml::entry<int**, float> >,
                       aml::dictionary< aml::entry<int, void>, aml::entry<int*, void*>, aml::entry<int**, float> > >::value, "");

        static_assert( std::is_same< d::add_entries< aml::entry<int***, float> >,
                       aml::dictionary
                       <
                           aml::entry<int, void>,
                           aml::entry<int*, void*>,
                           aml::entry<int**, void**>,
                           aml::entry<int***, float> >
                       >::value, "");


        static_assert( std::is_same
                       <
                           d::add_entries< aml::entry<int***, float>, aml::entry<int****, double> >,

                           aml::dictionary
                           <
                               aml::entry<int, void>,
                               aml::entry<int*, void*>,
                               aml::entry<int**, void**>,
                               aml::entry<int***, float>,
                               aml::entry<int****, double>
                           >
                       >::value, "");

        static_assert( std::is_same
                       <
                           d::add_entries< aml::entry<int****, double>, aml::entry<int***, float> >,

                           aml::dictionary
                           <
                               aml::entry<int, void>,
                               aml::entry<int*, void*>,
                               aml::entry<int**, void**>,
                               aml::entry<int****, double>,
                               aml::entry<int***, float>
                           >
                       >::value, "");

        static_assert( std::is_same
                       <
                           d::add_entries< aml::entry<int*, float>, aml::entry<int***, double> >,

                           aml::dictionary
                           <
                               aml::entry<int, void>,
                               aml::entry<int*, float>,
                               aml::entry<int**, void**>,
                               aml::entry<int***, double>
                           >
                       >::value, "");


        static_assert( std::is_same
                       <
                           d::add_entries< aml::entry<int***, double>, aml::entry<int*, float> >,

                           aml::dictionary
                           <
                               aml::entry<int, void>,
                               aml::entry<int*, float>,
                               aml::entry<int**, void**>,
                               aml::entry<int***, double>
                           >
                       >::value, "");


        // erase_keys
        // partially_erase_keys
    }
}


#include <iostream>
#include <string>


int main()
{

    void (*test_set[])() = { test::dictionary::empty_dictionary_test,
                             test::dictionary::filled_dictionary_test };


    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;

}
