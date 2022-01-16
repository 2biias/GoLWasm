#if !defined(RESETBUTTON_HPP)
#define RESETBUTTON_HPP

#include <gameEngine/SDLWrapper/renderer.hpp>
#include <gameEngine/SDLWrapper/color.hpp>
#include <gameEngine/gameComponent.hpp>
#include <utilities/math/vector2d.hpp>

namespace GameOfLife::Components
{

using namespace GameEngine::SDLWrapper;
using namespace Utilities;

template<typename Engine>
class ResetButton : public GameEngine::GameComponent, public std::enable_shared_from_this<ResetButton<Engine>>
{
public:
    ResetButton(Engine& engine, const Math::Vector2D& start, const uint32_t height, const uint32_t width)
    :   engine_(engine),
        start_(start),
        end_(start.getX()+width, start.getY()+height),
        backgroundColor_(136, 136, 136 )
    {
        resetEvent_ = SDL_RegisterEvents(1);
        engine_.getEventHandler()->addAllowedEvents(resetEvent_);
    }

    void registerInEngine() {
        id_ = engine_.registerComponent(this->shared_from_this());
    }

    void update() {
    }

    void render() {
        auto renderer_ptr = engine_.getRenderer();
        renderer_ptr->setRenderColor(backgroundColor_);
        renderer_ptr->drawRectangle(start_, end_);

        const std::string text = "Reset";
        renderer_ptr->drawText(start_, end_, Color(0xff, 0xfa, 0xfa), text);

    }

    void click(Math::Vector2D pos) {
        if (resetEvent_ != ((Uint32)-1)) {
            SDL_Event event;
            SDL_zero(event);
            event.type = resetEvent_;
            if(SDL_PushEvent(&event) != 1)
                throw std::runtime_error(std::string("Failed to push event ") + SDL_GetError());
        }
    }

    std::pair<Math::Vector2D, Math::Vector2D> getHitBox() const {
        return std::make_pair(start_, end_);
    }

    Uint32 getResetEvent() const { return resetEvent_; }

private:
    Engine& engine_;
    Math::Vector2D start_;
    Math::Vector2D end_;
    Color backgroundColor_;
    Uint32 resetEvent_;
    uint32_t id_;
};

template <typename Engine>
using ResetButtonTypeList = typename Meta::Append<COMPONENTS_LIST<Engine>, ResetButton<Engine>>::type;
#undef COMPONENTS_LIST
#define COMPONENTS_LIST ResetButtonTypeList

}

#endif // RESETBUTTON_HPP