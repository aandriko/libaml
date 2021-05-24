#include "aml/find.hpp"
#include "aml/conslist.hpp"

#include <type_traits>

#include <iostream>
#include <boost/core/demangle.hpp>

namespace test::find
{
  
    using l0 = aml::conslist<>;
    using l1 = aml::conslist<void, int, char*>;
    using l2 = aml::conslist<void, short int, char*, long int>;

    
    template<typename X>
    struct pred
    {
        static constexpr bool eval() { return std::is_integral<X>::value; };
    };
    
    using f0 = l0::apply<aml::find<pred>::in>;
    using f1 = l1::apply<aml::find<pred>::in>;
    
    using f2 = l2::apply<aml::find<pred>::in>;
    
    using r0 = aml::conslist<>;
    using r1 = aml::conslist<int>;
    using r2 = aml::conslist<short int>;
    
   
    void test()
    {
        static_assert(std::is_same<f0, r0>::value, "");
        static_assert(std::is_same<f1, r1>::value, "");
        static_assert(std::is_same<f2, r2>::value, "");                
    }
}


#include <iostream>
#include <string>


int main()
{
    void (*test_set[])() = { test::find::test };

    for ( auto test : test_set )
        test();

    std::cout << __FILE__ << ": " << sizeof(test_set)/sizeof(test_set[0])  << " tests passed." << std::endl;
}
