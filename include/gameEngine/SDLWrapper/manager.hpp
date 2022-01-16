#if !defined(MANAGER_HPP)
#define MANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace GameEngine::SDLWrapper
{

class Manager
{
public:
    template <typename... Flags>
    Manager(Flags &&... flags) {
        if( SDL_Init((flags | ...)) != 0 )
            throw std::runtime_error(std::string("Failed to initialise SDL ") + SDL_GetError());
        
        if( TTF_Init() != 0 )
            throw std::runtime_error(std::string("Failed to initialise TTF ") + SDL_GetError());
    }

    ~Manager() {
        SDL_Quit();
        TTF_Quit();
    }

};

}

#endif // MANAGER_HPP
