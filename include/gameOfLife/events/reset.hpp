#if !defined(RESET_HPP)
#define RESET_HPP

#include <SDL2/SDL.h>
#include <gameOfLife/components/cellManager.hpp>

namespace GameOfLife::Events
{

using namespace Utilities;
using namespace GameEngine::SDLWrapper;

template<typename Engine>
class Reset 
{
public:
    Reset(Engine& engine, std::shared_ptr<Components::CellManager<Engine>> cellManager)
    :   engine_(engine),
        cellManager_(cellManager)
    {}

    void operator()(SDL_Event& ev){
        cellManager_->resetCells();
    }
private:
    Engine& engine_;
    std::shared_ptr<Components::CellManager<Engine>> cellManager_;
};

}

#endif // RESET_HPP