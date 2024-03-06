#pragma once
#include <simplecpp>
#include "Vector2D.cpp"
#include "Draw.cpp"

class Shape : public Object, public Draw {
protected:
    Vector2D origin;
    float maxR;
public:
    virtual void moveShape(Vector2D &v) {}
    virtual void rotateShape (float angle) {}
    Vector2D* getOrigin() {
        return &origin;
    }

    float getMaxR() {
        return maxR;
    }
};

