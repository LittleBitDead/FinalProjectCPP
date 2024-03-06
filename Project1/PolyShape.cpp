#pragma once
#include <simplecpp>
#include "Vector2D.cpp"
#include "Draw.cpp"
#include "Shape.cpp"
#include "Line2D.cpp"

class PolyShape : public Shape {
protected:
    Vector2D *points;
    Line2D *lines;
    int n;

public:
    //absolute positioning with auto origin
    //at the center of mass (assuming uniform mass)
    PolyShape(Vector2D *_points, int _n) {
        points = _points;
        n = _n;

        lines = new Line2D[n];;

        float cX = 0;
        float cY = 0;
        float _area = 0;

        for (int i = 0; i < n; i++) {
            float temp = ((points + i)->getX() * (points + ((i + 1) % n))->getY()) -
                ((points + ((i + 1) % n))->getX() * (points + i)->getY());
            cX = cX + ((points + i)->getX() + (points + ((i + 1) % n))->getX()) * temp;
            cY = cY + ((points + i)->getY() + (points + ((i + 1) % n))->getY()) * temp;

            _area = _area + temp;

            lines[i] = Line2D(points[i],points[(i+1) % n]);
        }
         _area = _area/2.0f;

         cX = cX /(6.0f * _area);
         cY = cY /(6.0f * _area);

        //for (int i = 0; i < n; i++) {
        //    cX += points[i].getX();
        //   cY += points[i].getY();
        // }

        //cX = float(cX / n);
        //cY = float(cY / n);

        origin.setX(cX);
        origin.setY(cY);

        for (int i = 0; i < n; i ++) {
            float temp = Vector2D(points[i].getX() - origin.getX(), points[i].getY() - origin.getY()).magnitude();
            if (temp > maxR) {
                maxR = temp;
            }
            cout << maxR << endl;
        }
    }

    //Polyshape Centered around origin, vector points are just origin + points
    PolyShape(Vector2D *_points, int _n, Vector2D &_origin) {
        points = _points;
        origin = _origin;
        n = _n;
        lines = new Line2D[n];;

        float cX = 0;
        float cY = 0;
        float _area = 0;

        for (int i = 0; i < n; i++) {
            float temp = ((points + i)->getX() * (points + ((i + 1) % n))->getY()) -
                ((points + ((i + 1) % n))->getX() * (points + i)->getY());
            cX = cX + ((points + i)->getX() + (points + ((i + 1) % n))->getX()) * temp;
            cY = cY + ((points + i)->getY() + (points + ((i + 1) % n))->getY()) * temp;

            _area = _area + temp;

            lines[i] = Line2D(points[i],points[(i+1) % n]);
        }
         _area = _area/2.0f;

         cX = cX /(6.0f * _area);
         cY = cY /(6.0f * _area);

        Vector2D dis(origin.getX() - cX, origin.getY() - cY);

        for (int i = 0; i < n; i ++) {
            points[i].add(&dis);
            float temp = Vector2D(points[i].getX() - origin.getX(), points[i].getY() - origin.getY()).magnitude();
            if (temp > maxR) {
                maxR = temp;
            }
        }
        cout << maxR << endl;
    }

    void draw() override {
        for (int i = 0; i < n; i ++) {
            lines[i].draw();
        }
    }

    void moveShape(Vector2D &v) override {
        origin.add(&v);

        for (int i = 0; i < n; i++) {
            points[i].add(&v);
        }
    }

    void rotateShape(float angle) override {
        for (int i = 0; i < n; i ++) {
            points[i].sub(&origin);
            float newX = points[i].getX() * cos(angle) - points[i].getY() * sin(angle);
            float newY = points[i].getX() * sin(angle) + points[i].getY() * cos(angle);
            points[i].setX(newX);
            points[i].setY(newY);
            points[i].add(&origin);
        }
    }

    std::string toString() {
        std::stringstream out;
        out << "Shape: \n";

        for (int i = 0; i < n -1 ; i ++) {
            out << lines[i].toString() << endl;
        }
        out << lines[n-1].toString();
        out << std::endl << "Origin: ";
        out << origin.toString();
        return out.str();
    }
};

