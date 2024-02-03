#ifndef __BALL_H
#define __BALL_H

#include "raylib.h"
#include "raymath.h"
#include "Entity.h"
#include "Paddle.h"
#include "../Event.h"

class Ball : public Entity {
private:
    Texture2D *texture;
    const Rectangle *viewport;
    Paddle *paddle;
    bool launched = false;
    Vector2 velocity;
    EventHandler event;
    int frameCounter = 0;

public:
    Ball(Texture2D *texture, const Rectangle *viewport, Paddle *paddle, EventHandler event);
    ~Ball();
    void update();
    void draw();
    void launch();
    void reset();
    void invertVelocity();
};

#endif
