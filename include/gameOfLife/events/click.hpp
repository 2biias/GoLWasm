#if !defined(CLICK_HPP)
#define CLICK_HPP

#include <SDL2/SDL.h>

#include <utilities/math/vector2d.hpp>

namespace GameOfLife::Events
{

using namespace Utilities;
using namespace GameEngine::SDLWrapper;

template<typename Engine>
class Click 
{
public:
    Click(Engine& engine)
    :   engine_(engine)
    {}

    void click(SDL_Event& ev){
        
        auto& components = engine_.getComponents();
        auto buttonEvent = ev.button;
        
        for(auto& [id, component] : components) {
            std::visit([&](auto&& comp) {
                auto pos = comp->getHitBox();
                auto x = buttonEvent.x;
                auto y = buttonEvent.y;
                if(( x >= pos.first.getX() && x <= pos.second.getX() ) &&  (y >= pos.first.getY() && y <= pos.second.getY())) {
                    comp->click(Math::Vector2D(x, y));
                }
            }, component);
        }        
    }
private:
    Engine& engine_;
};

}
#endif // CLICK_HPP
