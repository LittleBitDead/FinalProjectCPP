#pragma once
#include "Object.cpp"
#include "Physics.cpp"
#include "Shape.cpp"

class GameObject : public Object, public Physics {
public:
    virtual void updatePhysics(float delta) {}

    virtual void updateGraphics() {}

    virtual Shape* getShape() {}

    virtual Vector2D* getVel() {
        return &velocity;
    }
};
