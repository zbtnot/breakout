#include "Block.h"

Block::Block(int x, int y, Texture2D *texture, Ball *ball) {
  this->dimensions = {
    .x = (float)x,
    .y = (float)y,
    .width = (float)texture->width,
    .height = (float)texture->height
  };

  this->texture = texture;
  this->ball = ball;

  this->active = true;
}

Block::~Block() {}

void Block::update() {
  if (!this->active) {
    return;
  }
}

void Block::draw() {
  if (!this->active) {
    return;
  }
  
  DrawTexture(*(this->texture), (int)this->dimensions.x, (int)this->dimensions.y, WHITE);
}
