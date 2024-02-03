#ifndef __BLOCK_H
#define __BLOCK_H

#include "Entity.h"
#include "Ball.h"
#include "raylib.h"

class Block : public Entity {
private:
    Texture2D *texture;
    Ball *ball;
public:
    Block(int x, int y, Texture2D *texture, Ball *ball);
    ~Block();
    void update();
    void draw();
};

#endif
