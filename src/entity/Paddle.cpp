#include "Paddle.h"

Paddle::Paddle(int x, int y, Texture2D *texture, const Rectangle *viewport, std::function<void(void)> launch) {
    this->dimensions = {
            .x = (float) x, .y = (float) y, .width = (float) texture->width, .height = (float) texture->height};
    this->texture = texture;
    this->viewport = viewport;
    this->active = true;
    this->aiming = true;
    this->launch = launch;
}

Paddle::~Paddle() {}

void Paddle::update() {
    if (!this->active) {
        return;
    }

    if (IsKeyDown(KEY_LEFT)) {
        this->dimensions.x -= 5;

        if (this->dimensions.x < 0) {
            this->dimensions.x = 0;
        }
    } else if (IsKeyDown(KEY_RIGHT)) {
        this->dimensions.x += 5;

        if (this->dimensions.x + this->dimensions.width > this->viewport->width) {
            this->dimensions.x = this->viewport->width - this->dimensions.width;
        }
    }

    if (this->aiming && IsKeyPressed(KEY_SPACE)) {
        this->launch();
    }
}

void Paddle::draw() {
    if (!this->active) {
        return;
    }

    DrawTexture(*(this->texture), (int) this->dimensions.x, (int) this->dimensions.y, WHITE);
}
