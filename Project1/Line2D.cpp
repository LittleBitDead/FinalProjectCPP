#pragma once

#include "Vector2D.cpp"
#include "Shape.cpp"
#include <simplecpp>

class Line2D : public Shape {
protected:
    Vector2D *startPoint;
    Vector2D *endPoint;
    Line line;

public:
    Line2D(Vector2D &_startPoint, Vector2D &_endPoint) {
        startPoint = &_startPoint;
        endPoint = &_endPoint;
        line = Line(startPoint->getX(), startPoint->getY(), endPoint->getX(), endPoint->getY());
    }
    Line2D() {
    }
    //Vector2D getEnd() {}
    //Vector2D getStart() {}
    void draw() {
        line.reset(startPoint->getX(), startPoint->getY(), endPoint->getX(), endPoint->getY());
    }
    std::string toString() {
        std::stringstream out;
        out << startPoint->toString() << "->" << endPoint->toString();
        return out.str();
    }
};
