#if !defined(CELL_HPP)
#define CELL_HPP

#include <gameEngine/gameComponent.hpp>
#include <gameEngine/SDLWrapper/renderer.hpp>
#include <gameEngine/SDLWrapper/color.hpp>
#include <gameOfLife/components/common.hpp>
#include <utilities/math/vector2d.hpp>



namespace GameOfLife::Components
{

using namespace GameEngine::SDLWrapper;
using namespace Utilities;

template<typename Engine>
class Cell : public GameEngine::GameComponent, public std::enable_shared_from_this<Cell<Engine>>
{
public:
    Cell(Engine& engine, const Math::Vector2D& pos, uint8_t size)
    :   engine_(engine),
        state_(CellState::alive),
        start_(pos.getX()*size+1, pos.getY()*size+1),
        end_(start_.getX()+size-1, start_.getY()+size-1),
        size_(size),
        cellColor_(0xff, 0xfa, 0xfa)
    {}

    void registerInEngine() {
        id_ = engine_.registerComponent(this->shared_from_this());
    }

    void kill() {
        engine_.unregisterComponent(id_);
    }

    void update() {
        //std::cout << "cell with pos: " << position_.getX() << "," << position_.getY() << " update called" << std::endl;
    }

    void render() {
        auto renderer_ptr = engine_.getRenderer();
        renderer_ptr->setRenderColor(cellColor_);
        renderer_ptr->drawRectangle(start_, end_);
    }

    void click(Math::Vector2D pos) {

    }

    std::pair<Math::Vector2D, Math::Vector2D> getHitBox() const {
        return std::make_pair(start_, end_);
    }

private:
    uint32_t id_;
    Engine& engine_;
    CellState state_;
    Math::Vector2D const start_;
    Math::Vector2D const end_;
    uint8_t const size_;
    Color const cellColor_;
};

template <typename Engine>
using CellTypeList = typename Meta::Append<COMPONENTS_LIST<Engine>, Cell<Engine>>::type;
#undef COMPONENTS_LIST
#define COMPONENTS_LIST CellTypeList

}

#endif // CELL_HPP