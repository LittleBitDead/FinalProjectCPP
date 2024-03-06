#pragma once
#include <simplecpp>
#include "Vector2D.cpp"
#include "Draw.cpp"
#include "Shape.cpp"
#include "Line2D.cpp"

class CircleShape : public Shape {
protected:
    Circle circle;
    float radius;

public:
    CircleShape(Vector2D &_origin, float _radius) {
        origin = _origin;
        radius = _radius;
        maxR = radius;
        circle = Circle(origin.getX(), origin.getY(), radius);
    }

    void draw() override {
        circle.moveTo(origin.getX(), origin.getY());
    }

    void moveShape(Vector2D &v) override {
        origin.add(&v);
    }

    std::string toString() {
        std::stringstream out;
        out << "Origin: " << origin.toString();
        return out.str();
    }
};
