#if !defined(COMPONENTSLIST_HPP)
#define COMPONENTSLIST_HPP

#include <utilities/meta/typelist.hpp>

using namespace Utilities;

namespace GameOfLife::Components
{

template<typename Engine>
using NullTypeList = typename Meta::NullType;
#define COMPONENTS_LIST NullTypeList

}

#endif // COMPONENTSLIST_HPP