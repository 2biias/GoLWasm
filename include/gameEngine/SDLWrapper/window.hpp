#if !defined(WINDOW_HPP)
#define WINDOW_HPP


#include <SDL2/SDL.h>

namespace GameEngine::SDLWrapper
{

class Window
{
public:
    Window(std::string&& name, const int height, const int width) {
        window_ = SDL_CreateWindow(
                name.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                height,
                width,
                0
            );
        if(window_ == nullptr)
            throw std::runtime_error(std::string("Failed to create window ") + SDL_GetError());
    }

    void cleanUp() {
        if (window_ != nullptr) {
            SDL_DestroyWindow(window_);
        }
    }

    SDL_Window* getWindow() const { return window_; }
    
    ~Window() {
        if (window_ != nullptr) {
            SDL_DestroyWindow(window_);
        }
    }

private:
    SDL_Window* window_;
};

}

#endif // WINDOW_HPP