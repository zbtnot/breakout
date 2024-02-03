#include "ScoreBoard.h"

ScoreBoard::ScoreBoard(const Rectangle *viewport, const Font *font, EventHandler event) {
    this->dimensions = {
        .x = 0,
        .y = 0,
        .width = viewport->width,
        .height = 40,
    };
    this->viewport = viewport;
    this->font = font;
    this->event = event;
}

ScoreBoard::~ScoreBoard() {}

void ScoreBoard::setScore(int score) { this->score = score; }

int ScoreBoard::getScore() { return this->score; }

void ScoreBoard::setLives(int lives) {
    this->lives = lives;
}

int ScoreBoard::getLives() {
    return this->lives;
}

void ScoreBoard::update() {
    if (lives <= 0) {
        this->event(Event::GAME_OVER, this);
    }

    this->scoreStream.str("");
    this->scoreStream << "Score: " << this->score;
    
    this->livesStream.str("");
    this->livesStream << "Lives: " << this->lives;
}

void ScoreBoard::draw() {
    auto livesStr = this->livesStream.str();
    auto position = Vector2 {
        .x = (float)(this->viewport->width - MeasureText(livesStr.c_str(), this->fontSize) + 5),
        .y = 2,
    };
    DrawRectangleRec(this->dimensions, Color { 0, 0, 0, 225 });
    DrawTextEx(*(this->font), this->scoreStream.str().c_str(), Vector2{10, 2}, this->fontSize, 0.0f, WHITE);
    DrawTextEx(*(this->font), livesStr.c_str(), position, this->fontSize, 0.0f, WHITE);
}

void ScoreBoard::reset()
{
    this->lives = 2;
    this->score = 0;
}