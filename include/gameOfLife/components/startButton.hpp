#if !defined(STARTBUTTON_HPP)
#define STARTBUTTON_HPP

#include <gameEngine/SDLWrapper/renderer.hpp>
#include <gameEngine/SDLWrapper/color.hpp>
#include <gameEngine/gameComponent.hpp>
#include <utilities/math/vector2d.hpp>


namespace GameOfLife::Components
{

using namespace GameEngine::SDLWrapper;
using namespace Utilities;


template<typename Engine>
class StartButton : public GameEngine::GameComponent, public std::enable_shared_from_this<StartButton<Engine>>
{
public:
    StartButton(Engine& engine, const Math::Vector2D& start, const uint32_t height, const uint32_t width)
    :   engine_(engine),
        start_(start),
        end_(start.getX()+width, start.getY()+height),
        backgroundColor_(136, 136, 136 )
    {
        startEvent_ = SDL_RegisterEvents(1);
        engine_.getEventHandler()->addAllowedEvents(startEvent_);
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

        const std::string text = "Start/Pause";
        renderer_ptr->drawText(start_, end_, Color(0xff, 0xfa, 0xfa), text);

    }

    void click(Math::Vector2D pos) {
        if (startEvent_ != ((Uint32)-1)) {
            SDL_Event event;
            SDL_zero(event);
            event.type = startEvent_;
            if(SDL_PushEvent(&event) != 1)
                throw std::runtime_error(std::string("Failed to push event ") + SDL_GetError());
        }
    }

    std::pair<Math::Vector2D, Math::Vector2D> getHitBox() const {
        return std::make_pair(start_, end_);
    }

    Uint32 getStartEvent() const { return startEvent_; }

private:
    Engine& engine_;
    Math::Vector2D start_;
    Math::Vector2D end_;
    Color backgroundColor_;
    Uint32 startEvent_;
    uint32_t id_;
};

template <typename Engine>
using StartButtonTypeList = typename Meta::Append<COMPONENTS_LIST<Engine>, StartButton<Engine>>::type;
#undef COMPONENTS_LIST
#define COMPONENTS_LIST StartButtonTypeList

}

#endif // STARTBUTTON_HPP