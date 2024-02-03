#include "Ball.h"
#include <iostream>

Ball::Ball(Texture2D *texture, const Rectangle *viewport, Paddle *paddle, std::function<void(void)> bonkHandler) {
    this->dimensions = {.width = (float) texture->width, .height = (float) texture->height};
    this->texture = texture;
    this->viewport = viewport;
    this->active = true;
    this->paddle = paddle;
    this->bonkHandler = bonkHandler;
}

Ball::~Ball() {}

void Ball::update() {
    if (!this->active) {
        return;
    }
    if (!this->launched) {
        auto centerOfPaddle = this->paddle->GetDimensions().x + this->paddle->GetDimensions().width / 2;
        auto x = centerOfPaddle - this->texture->width / 2;
        auto y = this->paddle->GetDimensions().y - this->texture->height;
        this->dimensions.x = x;
        this->dimensions.y = y;
    } else {
        this->dimensions.x += this->velocity.x;
        this->dimensions.y += this->velocity.y;

        if (this->dimensions.y <= 0) {
            this->dimensions.y = 0;
            this->velocity.y *= -1;
            this->bonkHandler();
        }
    
        // todo this should emit a ball lost event
        if (this->dimensions.y >= this->viewport->height - this->dimensions.height) {
            this->dimensions.y = this->viewport->height - this->dimensions.height;
            this->velocity.y *= -1;
        }

        if (CheckCollisionRecs(this->dimensions, this->paddle->GetDimensions())) {
            this->velocity.y *= -1;
            this->bonkHandler();
        }
    }
}

void Ball::draw() {
    if (!this->active) {
        return;
    }
    DrawTexture(*(this->texture), (int) this->dimensions.x, (int) this->dimensions.y, WHITE);
}

void Ball::launch() {
    this->launched = true;
    this->velocity = Vector2 {
        .x = 0,
        .y = -5,
    };
}
