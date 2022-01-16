#if !defined(RENDER_HPP)
#define RENDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <gameEngine/SDLWrapper/window.hpp>
#include <gameEngine/SDLWrapper/color.hpp>
#include <utilities/math/vector2d.hpp>

namespace GameEngine::SDLWrapper
{

using namespace Utilities;

class Renderer
{
public:
    Renderer(std::shared_ptr<Window>& window)
    :   renderer_(SDL_CreateRenderer(window->getWindow(), -1, SDL_RENDERER_ACCELERATED))
    {
        if(renderer_ == nullptr)
            throw std::runtime_error(std::string("Failed to create renderer ") + SDL_GetError());
        font_ = TTF_OpenFont("resources/OpenSans-Regular.ttf", 24);
        if(font_ == nullptr)
            std::cout << "ooooh" << std::endl;
    }

    void setRenderColor(const Color& color) {
        if(SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a) != 0)
            throw std::runtime_error(std::string("Failed to set renderColor ") + SDL_GetError());
    }

    void drawLine(const Math::Vector2D& startPoint, const Math::Vector2D& endPoint) {
        if(SDL_RenderDrawLine(renderer_, startPoint.getX(), startPoint.getY(), endPoint.getX(), endPoint.getY()) != 0)
            throw std::runtime_error(std::string("Failed to render line ") + SDL_GetError());
    }

    void drawRectangle(const Math::Vector2D& startPoint, const Math::Vector2D& endPoint) {
        Math::Vector2D diff = endPoint - startPoint;
        SDL_Rect rect = {startPoint.getX(), startPoint.getY(), diff.getX(), diff.getY()};
        if(SDL_RenderFillRect(renderer_, &rect) != 0)
            throw std::runtime_error(std::string("Failed to render rectangle ") + SDL_GetError());
    }

    void drawText(const Math::Vector2D& startPoint, const Math::Vector2D& endPoint, const Color& color, std::string text) {
        SDL_Color textColor = {color.r, color.g, color.b};
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font_, text.c_str(), textColor);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer_, surfaceMessage);

        Math::Vector2D diff = endPoint - startPoint;
        SDL_Rect Message_rect = {startPoint.getX(), startPoint.getY(), diff.getX(), diff.getY()};
        SDL_RenderCopy(renderer_, Message, NULL, &Message_rect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
    }

    void present() {
        SDL_RenderPresent(renderer_);
    }

    void clearRenderer() const {
        if(SDL_RenderClear(renderer_) != 0)
            throw std::runtime_error(std::string("Failed to clear renderer ") + SDL_GetError());
    }

    void cleanUp() {
        if (renderer_ != nullptr) {
            SDL_DestroyRenderer(renderer_);
        }
    }

    ~Renderer() {
        if (renderer_ != nullptr) {
            SDL_DestroyRenderer(renderer_);
        }
    }

private:
    SDL_Renderer* renderer_;
    TTF_Font* font_;
};

}

#endif // RENDER_HPP
