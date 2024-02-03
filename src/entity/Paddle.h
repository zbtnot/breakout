#ifndef __PADDLE_H
#define __PADDLE_H

#include "raylib.h"
#include "Entity.h"
#include "../Event.h"

class Paddle : public Entity {
private:
    Texture2D *texture;
    const Rectangle *viewport;
    bool aiming = false;
    EventHandler event;
    
public:
    Paddle(Texture2D *texture, const Rectangle *viewport, EventHandler event);
    ~Paddle();
    void update();
    void draw();
    void reset();
};

#endif
