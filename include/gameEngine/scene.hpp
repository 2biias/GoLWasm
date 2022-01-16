#if !defined(GAMESCENE_HPP)
#define GAMESCENE_HPP

namespace GameEngine
{

class GameScene
{
public:
    virtual void update() = 0;
    virtual void render() = 0;
};

}
#endif // GAMESCENE_H