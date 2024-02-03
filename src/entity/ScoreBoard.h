#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

#include <sstream>
#include "Entity.h"
#include "../Event.h"

class ScoreBoard : public Entity {
private:
    EventHandler event;
    const Rectangle *viewport;
    const Font *font;
    int score = 0;
    int lives = 2;
    int fontSize = 32;
    std::stringstream scoreStream;
    std::stringstream livesStream;
public:
    ScoreBoard(const Rectangle *viewport, const Font *font, EventHandler event);
    ~ScoreBoard();
    void setScore(int score);
    int getScore();
    void setLives(int lives);
    int getLives();
    void update();
    void draw();
    void reset();
};

#endif