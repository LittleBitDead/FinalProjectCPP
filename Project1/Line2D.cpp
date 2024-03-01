#pragma once

#include "Vector2D.cpp"
#include "Shape.cpp"

class Line2D : public Shape {
protected:
    Vector2D* startPoint;
    Vector2D* endPoint;

public:
    virtual std::string toString() {
        std::stringstream out;
        out << "(" << startPoint->getX() << "," << startPoint->getY() <<
        "(" << endPoint->getX() << "," << endPoint->getY() << ")";
        return out.str();
    }
};
