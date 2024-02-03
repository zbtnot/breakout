#ifndef __EVENT_H
#define __EVENT_H
#include <functional>

enum struct Event {
    BONK,
    BALL_LOST,
    LAUNCH,
    GAME_OVER,
    BLOCK_HIT,
};

typedef std::function<void(Event)> EventHandler;

#endif
