#if !defined(GAMECOMPONENT_HPP)
#define GAMECOMPONENT_HPP

#include <utilities/math/vector2d.hpp>

namespace GameEngine
{

using namespace Utilities;

class GameComponent
{
public:
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void registerInEngine() = 0;
    virtual void click(Math::Vector2D pos) = 0;
    virtual std::pair<Math::Vector2D, Math::Vector2D> getHitBox() const = 0;
    ~GameComponent() {}
};

}
#endif // GAMECOMPONENT_HPP