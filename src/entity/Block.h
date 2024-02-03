#ifndef __BLOCK_H
#define __BLOCK_H

#include "Entity.h"
#include "Ball.h"
#include "raylib.h"

class Block : public Entity {
private:
    Texture2D *texture;
    Ball *ball;
    EventHandler event;
public:
    Block(int x, int y, Texture2D *texture, Ball *ball, EventHandler event);
    ~Block();
    void update();
    void draw();
    void reset();
};

#endif
