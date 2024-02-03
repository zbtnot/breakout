#ifndef __PADDLE_H
#define __PADDLE_H

#include <functional>
#include "Entity.h"

#include "raylib.h"

class Paddle : public Entity {
private:
    Texture2D *texture;
    const Rectangle *viewport;
    bool aiming = false;
    std::function<void(void)> launch;
public:
    Paddle(int x, int y, Texture2D *texture, const Rectangle *viewport, std::function<void(void)> launch);
    ~Paddle();
    void update();
    void draw();
};

#endif
