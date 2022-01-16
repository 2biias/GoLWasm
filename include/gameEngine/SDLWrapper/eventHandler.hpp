#if !defined(EVENTHANDLER_HPP)
#define EVENTHANDLER_HPP

#include <map>
#include <vector>
#include <algorithm>
#include <boost/exception/all.hpp>
#include <exception>


#include <functional>
#include <SDL2/SDL.h>
#include <boost/signals2.hpp>

namespace GameEngine::SDLWrapper
{


class EventHandler
{
public:
    using EventToSignalMap = std::map<unsigned int, boost::signals2::signal<void (SDL_Event& )>>;

    EventHandler() {
        SDL_SetEventFilter(noEvents, this);
    }
    
    template <typename AllowedEvent, typename... Args>
    void addAllowedEvents(AllowedEvent event, Args ... args) {
        allowedEvents_.push_back(event);
        if constexpr(sizeof...(args) > 0)
            addAllowedEvents(args...);
        else
            SDL_SetEventFilter(filterEvents, this);
    }

    static int SDLCALL noEvents(void *data, SDL_Event * event) {
        return false;
    }

    static int SDLCALL filterEvents(void *data, SDL_Event * event) {
        EventHandler* eventHandler = static_cast<EventHandler*>(data);
        std::vector<unsigned int> allowedEvents = eventHandler->allowedEvents_;

        if(std::find(allowedEvents.begin(), allowedEvents.end(), event->type) != allowedEvents.end()) {
            return true;
        } else {
            return false;
        }
    }

    boost::signals2::connection addEventHandler(EventToSignalMap::key_type eventType, boost::function<void (SDL_Event&)> func) {
            // connect = strong exception guarantee
            eventToSignalMap_[eventType]; //construct signal (if not exits)
            boost::signals2::connection con;

            try {
                con = eventToSignalMap_[eventType].connect(func);
            } catch (boost::exception  &e){
                e << boost::error_info<struct tag_errmsg, std::string>{"failed to connect event"};
                throw; // rethrow
            }
            
            return con; // and connect it to func
    }

    void handleEvents() {   
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(eventToSignalMap_.count(event.type) > 0) {
                (eventToSignalMap_[event.type])(event);  //call signal and envoke all slots. basic guarantee.
            } else {
                // no handler attached.
            }
        }
    }
private:
    EventToSignalMap eventToSignalMap_;
    std::vector<unsigned int> allowedEvents_;
};

}

#endif // EVENTHANDLER_HPP
