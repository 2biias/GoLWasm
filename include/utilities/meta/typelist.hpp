#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <type_traits>
#include <variant>

namespace Utilities::Meta
{


/* Typelist */
struct NullType{};

template<typename L, typename R>
struct TypeList
{
    using First = L;
    using Rest = R;
};

/* Check for same types */
template<class T, class U>
struct IsSame : std::false_type {};

template<class T>
struct IsSame<T, T> : std::true_type {};

template<typename TL, typename T>
struct Contains
{
    static constexpr auto value = IsSame<typename TL::First, T>::value || Contains<typename TL::Rest, T>::value;
};

template<typename T>
struct Contains<NullType, T> : std::false_type
{};

template<typename ...Types>
struct Append
{
    using type = NullType; 
};

/* Append to nulltype (make typelist) */
template<typename T>
struct Append<NullType, T>
{
    using type = TypeList<T, NullType>;
};

/* Append to typelist */
template<typename TL, typename T>
struct Append<TL, T>
{
    using type = TypeList<T, TL>;
};

template<typename T, typename... Types>
struct PushVariant;

template<typename T, typename... Types>
struct PushVariant<T, std::variant<Types...>>
{
    using type = std::variant<T, Types...>;
};

template<typename TL>
struct TypeListToVariant;

template<typename L, typename R>
struct TypeListToVariant<TypeList<L, R>>
{
    using type = typename PushVariant<L, typename TypeListToVariant<R>::type>::type;
};

/*

template<typename L, typename R>
struct TypeListToVariant<TypeList<L, R>>
{
    using type = typename PushVariant<L, typename TypeListToVariant<R>::type>::type;
};

using type = typename PushVariant<L, PushVariant<L, PushVariant<L, std::variant<L>>::type >::type>::type;

using type = typename PushVariant<T1, std::variant<T2, std::variant<T3, std::variant<T4>> >::type>::type;

using type = std::variant<T1, T2, T3, T4>;

*/

template<typename L>
struct TypeListToVariant<TypeList<L, NullType>>
{
    using type = std::variant<L>;
};

template<typename T, typename... Types>
struct PushSharedVariant;

template<typename T, typename... Types>
struct PushSharedVariant<T, std::variant<Types...>>
{
    using type = std::variant<std::shared_ptr<T>, Types...>;
};

template<typename TL>
struct TypeListToSharedVariant;

template<typename L, typename R>
struct TypeListToSharedVariant<TypeList<L, R>>
{
    using type = typename PushSharedVariant<L, typename TypeListToSharedVariant<R>::type>::type;
};

template<typename L>
struct TypeListToSharedVariant<TypeList<L, NullType>>
{
    using type = std::variant<std::shared_ptr<L>>;
};

template<typename TL>
struct PrintIt
{
    PrintIt()
    {
        std::cout << typeid(typename TL::First).name() << "\n";
        PrintIt<typename TL::Rest> rest;
    }
};
template<>
struct PrintIt<NullType>
{};

}
#endif