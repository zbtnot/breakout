#include "Ball.h"

Ball::Ball(Texture2D *texture, const Rectangle *viewport, Paddle *paddle, EventHandler event) {
    this->dimensions = {.width = (float) texture->width, .height = (float) texture->height};
    this->texture = texture;
    this->viewport = viewport;
    this->active = true;
    this->paddle = paddle;
    this->event = event;
}

Ball::~Ball() {}

void Ball::update() {
    if (!this->active) {
        return;
    }
    
    // track the paddle until launch
    if (!this->launched) {
        auto centerOfPaddle = this->paddle->GetDimensions().x + this->paddle->GetDimensions().width / 2;
        auto x = centerOfPaddle - this->texture->width / 2;
        auto y = this->paddle->GetDimensions().y - this->texture->height;
        this->dimensions.x = x;
        this->dimensions.y = y;
    } else {
        this->dimensions.x += this->velocity.x;
        this->dimensions.y += this->velocity.y;
        auto ballCenterPoint = this->dimensions.x + this->dimensions.width / 2;
        auto ballVerticalPoint = this->dimensions.y + this->dimensions.height / 2; 

        // we need to account for the score banner
        if (this->dimensions.y <= 40) {
            this->dimensions.y = 40;

            if (ballCenterPoint < this->viewport->width / 2) {
                this->velocity.x = 2;
            } else {
                this->velocity.x = -2;
            }
            this->event(Event::BONK, this);
        }

        // handle the sides
        if (this->dimensions.x <= 0 || this->dimensions.x >= this->viewport->width - this->dimensions.width) {
            if (ballVerticalPoint < this->viewport->height / 2) {
                this->velocity.y *= -1;
            } 
            this->event(Event::BONK, this);
            if (ballVerticalPoint > this->viewport->height / 2) {
                this->velocity.y *= -1;
            }
        }

        if (this->dimensions.y >= this->viewport->height - this->dimensions.height) {
            this->event(Event::BALL_LOST, this);
            return;
        }

        if (CheckCollisionRecs(this->dimensions, this->paddle->GetDimensions())) {
            auto paddleCenterPoint = this->paddle->GetDimensions().x + this->paddle->GetDimensions().width / 2;
            const int offset = 5; // 5 pixels on each side to allow for a straight shot in the center of the paddle

            if (ballCenterPoint + offset < paddleCenterPoint) {
                this->velocity.x = 2;
            } else if (ballCenterPoint - offset > paddleCenterPoint) {
                this->velocity.x = -2;
            } else {
                this->velocity.x = 0;
            }

            this->event(Event::BONK, this);
        }

        frameCounter = 0;
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
    this->velocity = Vector2{
            .x = 0,
            .y = -5,
    };
}

void Ball::reset() {
    this->launched = false;
    this->velocity = Vector2Zero();
}

void Ball::invertVelocity() {
    // only invert once per frame
    if (frameCounter++ == 0) {
        this->velocity.x *= -1;
        this->velocity.y *= -1;
    }
}