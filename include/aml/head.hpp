#pragma once


namespace aml::lazy
{
    template<typename...>
    struct head;

    
    template<typename H, typename... T>
    struct head<H, T...>
    {
        using type = H;
    };
}


namespace aml
{
    template<typename... X>
    using head = typename lazy::head<X...>::type;
}
    
    
