#pragma once
#include <simplecpp>
#include "Physics2D.cpp"
#include "Vector2D.cpp"
#include "Draw.cpp"
#include "Shape.cpp"
#include "Line2D.cpp"

#define loopPoints(pointsArrayPointer) for (Vector2D *p = pointsArrayPointer; p <= pointsArrayPointer + sizeof(Vector2D)/sizeof(pointsArrayPointer); ++p)

class PolyShape : public Shape {
protected:
    Vector2D origin;
    Vector2D* points;
    Line2D* lines;

public:

    //absolute positioning with auto origin
    //at the center of mass (assuming uniform mass)
    PolyShape(Vector2D& _points) {
        this->points = &_points;
        int n = (sizeof(Vector2D)/sizeof(points) + 1);
        float cX = 0;
        float cY = 0;
        float area = 0;

        for (int i = 0; i < n; i++) {
            float temp = ((points + i)->getX() * (points + ((i + 1) % n))->getY()) -
            ((points + ((i + 1) % n))->getX() * (points + i)->getY());

            cX = cX + ((points + i)->getX() + (points + ((i + 1) % n))->getX()) * temp;
            cY = cY + ((points + i)->getY() + (points + ((i + 1) % n))->getY()) * temp;

            area = area + temp;
        }
        area = area/2.0f;

        cX = cX /(6.0f * area);
        cY = cY /(6.0f * area);
        cout << "CX: " << cX << ", CY: " << cY << endl;
        origin.setX(cX);
        origin.setY(cY);
    }

    //Polyshape Centered around 0,0 then moved to origin
    PolyShape(Vector2D& _points, Vector2D& origin) {
        cout << "IMPLEMENT" << endl;
    }

    void draw() {
        std::cout << "Drawing";
    }

    std::string toString() {
        std::stringstream out;
        out << "Shape: ";
        loopPoints(points) {
            out << p->toString() << ", ";
        }
        out << std::endl << "Origin: ";
        out << origin.toString();
        return out.str();
    }
};

