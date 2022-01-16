#if !defined(APPLICATION_HPP)
#define APPLICATION_HPP

#define MENUHEIGHT 100
#define CELLSPRAXIS 40

/* External includes */
#include <SDL.h>
#include <thread>
#include <chrono>

/* Internal includes */
#include<gameOfLife/components/componentVariant.hpp>
#include <gameEngine/SDLWrapper/renderer.hpp>
#include <gameEngine/engine.hpp>
#include <gameEngine/controller.hpp>
#include <gameOfLife/grid.hpp>
#include <gameOfLife/components/cellManager.hpp>
#include <gameOfLife/components/startButton.hpp>
#include <gameOfLife/components/resetButton.hpp>
#include<gameOfLife/components/componentVariant.hpp>

#include <gameOfLife/events/click.hpp>
#include <gameOfLife/events/start.hpp>
#include <gameOfLife/events/reset.hpp>

namespace GameOfLife
{

using namespace Utilities;

class Application
{
public:
    using Engine = GameEngine::Engine<Grid, Components::COMPONENT_SHARED_VARIANT>;

    Application(const uint32_t cellsPrAxis, const uint32_t cellSize)
    :   engine_("Game Of Life", cellsPrAxis*cellSize+1, cellsPrAxis*cellSize+1+MENUHEIGHT, SDL_INIT_VIDEO, SDL_INIT_EVENTS, SDL_INIT_TIMER),
        grid_(engine_, cellsPrAxis, cellSize),
        controller_(engine_),
        cellManager_(std::make_shared<Components::CellManager<Engine>>(engine_, cellsPrAxis, cellSize)),
        startButton_(std::make_shared<Components::StartButton<Engine>>(engine_, Math::Vector2D(5, cellsPrAxis*cellSize+5), MENUHEIGHT/2-5, cellsPrAxis*cellSize/2-5)),
        resetButton_(std::make_shared<Components::ResetButton<Engine>>(engine_, Math::Vector2D(cellsPrAxis*cellSize/2+5, cellsPrAxis*cellSize+5), MENUHEIGHT/2-5, cellsPrAxis*cellSize/2-10)),
        click_(std::make_shared<Events::Click<Engine>>(engine_)),
        start_(std::make_shared<Events::Start<Engine>>(engine_, cellManager_)),
        reset_(std::make_shared<Events::Reset<Engine>>(engine_, cellManager_))
    {
        cellManager_->registerInEngine();
        startButton_->registerInEngine();
        resetButton_->registerInEngine();
        setupEvents();
    }

    void start() {
        controller_.start();
    }

    void setupEvents() {
        
        auto eventHandler = engine_.getEventHandler();

        eventHandler->addAllowedEvents(SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP, SDL_QUIT, SDL_USEREVENT);

        //use with member function and bind
        try {
            eventHandler->addEventHandler(SDL_MOUSEBUTTONDOWN, std::bind(&Events::Click<Engine>::click, click_, std::placeholders::_1));
        } catch (boost::exception &e) {
            std::cerr << boost::diagnostic_information(e);
        }

        //use with lambda and connection return value
        auto stopGame = [&](SDL_Event& ev){ controller_.stop(); };
        auto quit_connect = eventHandler->addEventHandler(SDL_QUIT, stopGame);

        //use with functor
        eventHandler->addEventHandler(startButton_->getStartEvent(), *start_);
        eventHandler->addEventHandler(resetButton_->getResetEvent(), *reset_);
    }

    ~Application(){}

private:
    Engine engine_;
    GameEngine::Controller<Engine> controller_;
    Grid<Engine> grid_;

    std::shared_ptr<Components::CellManager<Engine>> cellManager_;
    std::shared_ptr<Components::StartButton<Engine>> startButton_;
    std::shared_ptr<Components::ResetButton<Engine>> resetButton_;
    std::shared_ptr<Events::Click<Engine>> click_;
    std::shared_ptr<Events::Start<Engine>> start_;
    std::shared_ptr<Events::Reset<Engine>> reset_;
};

}

#endif // APPLICATION_HPP

