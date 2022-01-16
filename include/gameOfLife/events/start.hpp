#if !defined(START_HPP)
#define START_HPP

#include <SDL2/SDL.h>
#include <gameOfLife/components/cellManager.hpp>

namespace GameOfLife::Events
{

using namespace Utilities;
using namespace GameEngine::SDLWrapper;

template<typename Engine>
class Start 
{
public:
    Start(Engine& engine, std::shared_ptr<Components::CellManager<Engine>> cellManager)
    :   engine_(engine),
        cellManager_(cellManager)
    {}

    void operator()(SDL_Event& ev){
        cellManager_->startPause();
    }
private:
    Engine& engine_;
    std::shared_ptr<Components::CellManager<Engine>> cellManager_;
};

}

#endif // START_HPP