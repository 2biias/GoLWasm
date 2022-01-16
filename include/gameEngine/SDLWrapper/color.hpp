#if !defined(COLOR_HPP)
#define COLOR_HPP

#include <SDL2/SDL.h>

namespace GameEngine::SDLWrapper
{

class Color : public SDL_Color
{
public:
    Color(uint8_t r, uint8_t g, uint8_t b) : SDL_Color{r, g, b, 255}
    {}
    
    Color(const SDL_Color& color) : SDL_Color{color.r, color.g, color.b, color.a}
    {}
};

}

#endif // COLOR_HPP
