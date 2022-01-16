#if !defined(COMPONENTVARIANT_HPP)
#define COMPONENTVARIANT_HPP


#include <utilities/meta/typelist.hpp>
#include <gameOfLife/components/componentList.hpp>
#include <gameOfLife/components/cell.hpp>
#include <gameOfLife/components/cellManager.hpp>
#include <gameOfLife/components/startButton.hpp>
#include <gameOfLife/components/resetButton.hpp>

using namespace Utilities;

namespace GameOfLife::Components
{

template<typename Engine>
using COMPONENT_VARIANT = typename Meta::TypeListToVariant<COMPONENTS_LIST<Engine>>::type;
template<typename Engine>
using COMPONENT_SHARED_VARIANT = typename Meta::TypeListToSharedVariant<COMPONENTS_LIST<Engine>>::type;

}
#endif // COMPONENTVARIANT_HPP