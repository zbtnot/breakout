#include "Paddle.h"
#include <iostream>

Paddle::Paddle(Texture2D *texture, const Rectangle *viewport, EventHandler event) {
    auto x = viewport->width / 2 - texture->width / 2;
    auto y = viewport->height - 50;
    this->dimensions = {
            .x = (float) x, .y = (float) y, .width = (float) texture->width, .height = (float) texture->height};
    this->texture = texture;
    this->viewport = viewport;
    this->active = true;
    this->aiming = true;
    this->event = event;
}

Paddle::~Paddle() {}

void Paddle::update() {
    if (!this->active) {
        return;
    }

    bool moveLeft = false;
    bool moveRight = false;
    bool launch = false;

    if (
        IsKeyDown(KEY_LEFT)
        || (IsGamepadAvailable(0) && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (int)(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X)) == -1))
    ) {
        moveLeft = true;
    }
    
    if (
        IsKeyDown(KEY_RIGHT) 
        || (IsGamepadAvailable(0) && (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (int)(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X)) == 1))
    ) {
        moveRight = true;
    }
    
    if (IsKeyPressed(KEY_SPACE) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) {
        launch = true;
    }

    if (moveLeft) {
        this->dimensions.x -= 5;

        if (this->dimensions.x < 0) {
            this->dimensions.x = 0;
        }
    } else if (moveRight) {
        this->dimensions.x += 5;

        if (this->dimensions.x + this->dimensions.width > this->viewport->width) {
            this->dimensions.x = this->viewport->width - this->dimensions.width;
        }
    }

    if (this->aiming && launch) {
        this->event(Event::LAUNCH, this);
        this->aiming = false;
    }
}

void Paddle::draw() {
    if (!this->active) {
        return;
    }

    DrawTexture(*(this->texture), (int) this->dimensions.x, (int) this->dimensions.y, WHITE);
}

void Paddle::reset() {
    this->aiming = true;
    auto x = this->viewport->width / 2 - this->texture->width / 2;
    auto y = this->viewport->height - 50;
    this->dimensions = {.x = (float) x,
                        .y = (float) y,
                        .width = (float) this->texture->width,
                        .height = (float) this->texture->height};
}
