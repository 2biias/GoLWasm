#if !defined(GRID_HPP)
#define GRID_HPP

/* Internal includes */
#include <gameEngine/SDLWrapper/color.hpp>
#include <gameEngine/engine.hpp>
#include <gameEngine/scene.hpp>

#include <utilities/math/vector2d.hpp>

namespace GameOfLife
{

using namespace GameEngine::SDLWrapper;

template<typename Engine>
class Grid : public GameEngine::GameScene
{
public:
    Grid(Engine& engine, const uint32_t cellsPrAxis, const uint32_t cellSize)
    :   engine_(engine),
        backgroundColor_(22, 22, 22),
        lineColor_(44, 44, 44),
        cellSize_(cellSize),
        gridWidth_((cellSize * cellsPrAxis) + 1),
        gridHeight_((cellSize * cellsPrAxis) + 1)
    {
        engine_.registerScene(std::move(*this));
    }

    void update() {
        /* Nothing for now */
    }

    void render() {
        auto renderer_ptr = engine_.getRenderer();
        renderer_ptr->setRenderColor(backgroundColor_);
        renderer_ptr->clearRenderer();
        renderer_ptr->setRenderColor(lineColor_);
        for(size_t x = 0; x < gridWidth_; x += cellSize_)
        {
            Utilities::Math::Vector2D v_start(x, 0);
            Utilities::Math::Vector2D v_end(x, gridHeight_);
            renderer_ptr->drawLine(v_start, v_end);
        }
        for(size_t y = 0; y < gridHeight_; y += cellSize_)
        {
            Utilities::Math::Vector2D v_start(0, y);
            Utilities::Math::Vector2D v_end(gridWidth_, y);
            renderer_ptr->drawLine(v_start, v_end);
        }
    }

    uint32_t getWindowWidth() const  { return gridWidth_; }
    uint32_t getWindowHeight() const { return gridHeight_; }
    Color getBackColor() const { return backgroundColor_; }
    Color getLineColor() const { return lineColor_; }

private:
    Engine& engine_;
    uint32_t cellSize_;
    uint32_t gridWidth_;
    uint32_t gridHeight_;
    Color backgroundColor_;
    Color lineColor_;
};

}
#endif // GRID_HPP
