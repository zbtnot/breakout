#ifndef __ENTITY_H
#define __ENTITY_H

#include "raylib.h"

/**
 * Represents a basic drawable and updatable game component
 */
class Entity {
protected:
    Rectangle dimensions;
    bool active = false;

public:
    virtual void update() = 0;
    virtual void draw() = 0;

    Rectangle &GetDimensions() { return this->dimensions; }

    bool &GetActive() { return this->active; }
};

#endif