#if !defined(GAMECONTROLLER_HPP)
#define GAMECONTROLLER_HPP

#include <functional>
#include <iostream>
#include <SDL.h>
#include <emscripten.h>

template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
   template <typename... Args> 
   static Ret callback(Args... args) {                    
      return func(args...);  
   }
   static std::function<Ret(Params...)> func; 
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

typedef void (*callback_t)(void);

namespace GameEngine
{

#define GAME_TICK_MS 50

template<typename Engine>
class Controller
{
public:
    Controller(Engine& engine)
    : engine_(engine)
    {}

    void start() {
        
        #ifdef __EMSCRIPTEN__
        Callback<void (void)>::func = std::bind(&Controller::run, this);
        callback_t func = static_cast<callback_t>(Callback<void (void)>::callback);
        emscripten_set_main_loop(func, -1, 10);
        #else
        while(running_ == true) {
            run();
            tick();
        }
        #endif
    }

    void run() {
        engine_.update();
        engine_.render();
    }

    void tick() { SDL_Delay(GAME_TICK_MS); }

    void stop() { running_ = false; }

private:
    bool running_;
    Engine& engine_;
};

}
#endif // GAMECONTROLLER_HPP