#ifndef __EVENT_H
#define __EVENT_H
#include <functional>
#include "entity/Entity.h"

enum struct Event {
    BONK,
    BALL_LOST,
    LAUNCH,
    GAME_OVER,
    BLOCK_HIT,
};

typedef std::function<void(Event, Entity *)> EventHandler;

#endif
