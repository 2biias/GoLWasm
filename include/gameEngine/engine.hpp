#if !defined(GAMEENGINE_HPP)
#define GAMEENGINE_HPP

/* External includes */
#include <vector>
#include <map>

/* Internal includes */
#include <gameEngine/SDLWrapper/renderer.hpp>
#include <gameEngine/SDLWrapper/window.hpp>
#include <gameEngine/SDLWrapper/manager.hpp>
#include <gameEngine/SDLWrapper/eventHandler.hpp>

#include <gameOfLife/grid.hpp>

namespace GameEngine
{

template< template<typename T> typename SceneType, template<typename T> typename ComponentType>
class Engine
{
public:
    using Scene = SceneType<Engine>;
    using Component = ComponentType<Engine>;

    template <typename... Flags>
    Engine(std::string name, const int height, const int width, Flags&&... flags)
    :   manager_(std::make_unique<SDLWrapper::Manager>(std::forward<Flags>(flags)...)),
        window_(std::make_shared<SDLWrapper::Window>(std::move(name), height, width)),
        renderer_(std::make_shared<SDLWrapper::Renderer>(window_)),
        eventHandler_(std::make_shared<SDLWrapper::EventHandler>())
    {}

    void update() {
        /* Loop through gameComponets and update these */
        for(auto& [id, component] : idToComponentMap_) {
            std::visit([](auto&& arg){ arg->update(); }, component);
        }
        scene_->update();
        handleEvents();
    }

    void render() {
        /* Loop through gameComponets and render these */
        scene_->render();
        
        for(auto& [id, component] : idToComponentMap_) {
            std::visit([](auto&& arg){ arg->render(); }, component);
        }
        renderer_->present();
    }

    void handleEvents() {
        eventHandler_->handleEvents();
    }

    void registerScene(Scene&& scene) {
        scene_ = std::make_unique<Scene>(std::move(scene));
    }

    uint32_t registerComponent(Component component) {
        idToComponentMap_[idCount_] = component;
        return idCount_++;
    }

    void unregisterComponent(uint32_t id) {
        if(idToComponentMap_.count(id) > 0)
            idToComponentMap_.erase(id);
    }

    auto getRenderer() const { return renderer_; }

    auto getWindow() const { return window_; }

    auto getEventHandler() const { return eventHandler_; }
 
    auto& getComponents() { return idToComponentMap_; }

    ~Engine()
    {
        renderer_->cleanUp();
        window_->cleanUp();
    }

private:
    std::unique_ptr<SDLWrapper::Manager> manager_;
    std::shared_ptr<SDLWrapper::Window> window_;
    std::shared_ptr<SDLWrapper::Renderer> renderer_;
    std::shared_ptr<SDLWrapper::EventHandler> eventHandler_;
    
    std::unique_ptr<Scene> scene_;
    std::map<uint32_t, Component> idToComponentMap_;
    uint32_t idCount_ = 0;
};


}

#endif // GAMEENGINE_HPP