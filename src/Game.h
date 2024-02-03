#ifndef __GAME_H
#define __GAME_H

#include "entity/Ball.h"
#include "entity/Paddle.h"
#include "entity/Block.h"
#include "entity/ScoreBoard.h"
#include "Event.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <sstream>

class Game {
private:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const Rectangle VIEWPORT = {
            .x = 0, .y = 0, .width = (float) this->SCREEN_WIDTH, .height = (float) this->SCREEN_HEIGHT};

    Sound bonk;
    Sound womp;
    Music bgm;
    Music gameOverBgm;
    Texture2D bg;
    Texture2D paddleTexture;
    Texture2D ballTexture;
    std::vector<Texture2D> blockTextures;
    Font font;
    Paddle *paddle;
    Ball *ball;
    std::vector<Block *> blocks;
    ScoreBoard *scoreBoard;
    bool gameOver = false;
    std::stringstream gameOverMessage;

public:
    Game(const char *title);
    ~Game();
    void frame();
    void update();
    void draw();
    void handleEvents(Event e);
};

#endif
