#pragma once


namespace aml::adt::dtl
{
    template<bool>
    struct lazy_bool;

    template<>
    struct lazy_bool<true>
    {
        struct type;
    };

    template<bool b>
    using sfinae = typename lazy_bool<b>::type;  // same as std::enable_if_t<b>
}


namespace aml::adt
{    
    template<typename...> class linker;
    
    template<template<typename...> class... Subtypes>
    struct subtypes_
    {
        template<typename... x>
        using link_with_parameters = typename linker< subtypes_<Subtypes...>, x...>::algebraic_type;
    };

    
    template<typename... Subtypes>
    using subtypes = subtypes_<Subtypes::template apply_to...>;
        
    
    template
    <
        typename...                    Parameters,
        template<typename...> class... Subtypes
    >
    struct linker< subtypes_<Subtypes...>, Parameters... >
    {
        struct algebraic_type
        :
            public Subtypes< linker<subtypes_<Subtypes...>, Parameters... >, Parameters... >...
        {
            constexpr algebraic_type() = default;

            template<typename Other>
            constexpr algebraic_type(Other&& other)
            :
                Subtypes< linker<subtypes_<Subtypes...>, Parameters... >, Parameters... >(static_cast<Other&&>(other))...
            { }

            template<typename... Args, typename dtl::sfinae< (sizeof...(Args) > 1) > >
            constexpr algebraic_type(Args&&... args)
            :
                Subtypes< linker<subtypes_<Subtypes...>, Parameters... >, Parameters... >(static_cast<Args&&>(args))...
            { }

            algebraic_type& operator=(algebraic_type const&) = default;
            algebraic_type& operator=(algebraic_type&&)      = default;                
        };

        template<typename... x>
        using abstract_type = typename linker< subtypes_<Subtypes...>, x... >::algebraic_type;
        
    };  
}
