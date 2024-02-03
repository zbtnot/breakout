#ifndef __BALL_H
#define __BALL_H

#include <functional>
#include "Entity.h"
#include "Paddle.h"
#include "raylib.h"
#include "raymath.h"

class Ball : public Entity {
private:
    Texture2D *texture;
    const Rectangle *viewport;
    Paddle *paddle;
    bool launched = false;
    Vector2 velocity;
    std::function<void(void)> bonkHandler;
public:
    Ball(Texture2D *texture, const Rectangle *viewport, Paddle *paddle, std::function<void(void)> bonkHandler);
    ~Ball();
    void update();
    void draw();
    void launch();
};

#endif
